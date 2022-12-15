classdef M_SegSVM
    % Segment-based SVM
    % By: Minh Hoai Nguyen (minhhoai@cs.stonybrook.edu)
    % Created: 25-Feb-2016
    % Last modified: 25-Feb-2016
    
    methods (Static)
        % Train a segment-based SVM
        %   The positve samples are the gt events, and possibly truncated gt events
        %   The negative samples are random segments that do not overlap with gt events.
        % Inputs:
        %   Ds: n*1 cell structure of time series, Ds{i} is d*n_i matrix
        %   lbs: 2*n matrix for gt labels, lbs(:,i) is 2*1 vector for event's onset and offset
        %   C: C for SVM
        %   truncRatios: 1*k vector whose entries are between 0 and 1.
        %       This tells us the proportions of gt events are used as positive samples
        %       Default value is [1];
        %   cnstrOpt: options for constraints. Only two fields are used: minSegLen and maxSegLen
        %   nNegPerTS: number of negative segments per time series. Default is 20.
        % Outputs:
        %   w, b: for linear SVM
        function [w, b] = train(Ds, lbs, featOpts, C, truncRatios, mvOpts, nNegPerTS)            
            if ~exist('truncRatios', 'var') || isempty(truncRatios)
                truncRatios = 1;
            end;
            
            if ~exist('nNegPerTS', 'var') || isempty(nNegPerTS)
                nNegPerTS = 20; % number of negative samples per time series
            end;
            nNegPerTS_o2 = ceil(nNegPerTS/2);
                        
            n = length(Ds);            
            fd = featOpts.dim;
            posFeatVecs = cell(1, n);
            negFeatVecs = cell(1, n);
            for i=1:n
                D = Ds{i};
                lb = lbs(:,i);
                len_i = lb(2) - lb(1) + 1;
                if (lb(1) > 0) % contain the event of interest
                    posVecs = zeros(fd, length(truncRatios));
                    for k=1:length(truncRatios)
                        truncLen = floor(truncRatios(k)*len_i);
                        posVecs(:,k) = featOpts.func(D(:, lb(1):(lb(1)+truncLen-1)));
                    end;
                    posFeatVecs{i} = posVecs;
                end;
                
                negFeatVecs_i1 = M_SegSVM.getRandSegFeatVecs(D(:,1:lb(1)-1), nNegPerTS_o2, featOpts, mvOpts);
                negFeatVecs_i2 = M_SegSVM.getRandSegFeatVecs(D(:,lb(2)+1:end), nNegPerTS_o2, featOpts, mvOpts);
                negFeatVecs{i} = cat(2, negFeatVecs_i1, negFeatVecs_i2);
            end;
            
            posD = cat(2, posFeatVecs{:});
            negD = cat(2, negFeatVecs{:});
            nPos = size(posD, 2);
            nNeg = size(negD, 2);
            
            trLb = [ones(nPos,1); -ones(nNeg, 1)];
            TrD = [posD, negD];
            
            % Include option for reweighting two classes
            opts = sprintf('-t 0 -c %g', C/size(TrD,2)); 
            model = svmtrain(trLb, TrD, opts);
            w = model.SVs'*model.sv_coef;
            b = - model.rho;            
        end

        % Inputs:
        %   SegD: d*n the raw data of a time series
        %   nSample: number of random segments to get
        %   featOpts, mvOpts: see m_params
        % Output:
        %   featVecs: d*nSample matrix for nSample feature vectors
        function featVecs = getRandSegFeatVecs(SegD, nSample, featOpts, mvOpts)
            n = size(SegD, 2);
            if (n == 0)
                featVecs = [];
                return;
            end;
            
            if ~exist('mvOpts', 'var') || isempty(mvOpts)
                mvOpts.lmin = 1;
                mvOpts.lmax = intmax('int32');
                mvOpts.swStep = 1;
            end;
            
            [segStarts, segEnds] = meshgrid(1:mvOpts.swStep:n, 1:mvOpts.swStep:n);
            segLens = segEnds - segStarts  + 1;
            validIdxs = and(segLens >= mvOpts.lmin, segLens <= mvOpts.lmax);
            if isempty(~find(validIdxs))
                featVecs = [];
                return;
            end;
            randSegs = [segStarts(validIdxs) segEnds(validIdxs)]';
            randNums = randi(size(randSegs,2), [1, nSample]);
            randSegs = randSegs(:, randNums);
            
            fd = featOpts.dim; 
            featVecs = zeros(fd, nSample);
            for i=1:nSample
                featVecs(:,i) = featOpts.func(SegD(:, randSegs(1,i):randSegs(2,i)));
            end
        end
        
        % Inputs:
        %   Ds: n*1 cell structure of time series, Ds{i} is d*n_i matrix
        %   lbs: 2*n matrix for gt labels, lbs(:,i) is 2*1 vector for event's onset and offset
        %   featOpts, detOpts: see m_params for explanation
        %   w, b: parameters of linear SVM model
        %   amocOpt: option for AMOC curve, either 'mean' or 'median', default is 'median'
        % Outputs:
        %   rocArea:
        %   xamoc, yamoc: performance curves
        function [rocArea, xamoc, yamoc, detectScores] = ...
                test(Ds, lbs, featFunc, detOpts, w, b, amocOpt)
            
            nTst = length(Ds);
            detectScores = cell(1, nTst);
            
            startT = tic;
            for i=1:nTst
                ml_progressBar(i, nTst, 'Testing sequence', startT);
                %detectOut = M_MMED.eval(w, b, Ds{i}, featFunc, detOpts);
                detectOut = M_MMED.eval_fast(w, b, Ds{i}, featFunc, detOpts);
                detectScores{i} = detectOut(3,:);
            end;
            
            [~, ~, rocArea]   = m_getPerfCurve(detectScores, lbs,'ROC');
            if ~exist('amocOpt', 'var') || isempty(amocOpt) || strcmpi(amocOpt, 'AMOC-median')
                [xamoc, yamoc] = m_getPerfCurve(detectScores, lbs,'AMOC');
            elseif strcmpi(amocOpt, 'AMOC-mean')
                [xamoc, yamoc] = m_getPerfCurve(detectScores, lbs,'AMOC-mean');
            else
                error('m_tstSegSVM.m: unknown amocOpt');
            end;
        end
        
        % Find the frame number of that the detector fires.
        % Inputs:
        %   Ds: n*1 cell structure of time series, Ds{i} is d*n_i matrix
        %   lbs: 2*n matrix for gt labels, lbs(:,i) is 2*1 vector for event's onset and offset
        %   kOpt: kernel option
        %   sOpt: search and segment option
        %   w, b: parameters of linear SVM model
        %   amocOpt: option for AMOC curve, either 'mean' or 'median', default is 'median'
        % Outputs:
        %   fireFrames: the frame numbers at which the detector fires
        function fireFrames = fireFrame_at_FPR(Ds, lbs, featFunc, detOpts, w, b, amocOpt, targetFPR)
            nTst = length(Ds);
            detectScores = cell(1, nTst);
            
            for i=1:nTst
                detectOut = M_MMED.eval(w, b, Ds{i}, featFunc, detOpts);
                detectScores{i} = detectOut(3,:);
            end;
            
            if ~exist('amocOpt', 'var') || isempty(amocOpt) || strcmpi(amocOpt, 'AMOC-median')
                [xamoc, ~, ~, threshVals] = m_getPerfCurve(detectScores, lbs,'AMOC');
            elseif strcmpi(amocOpt, 'AMOC-mean')
                [xamoc, ~, ~, threshVals] = m_getPerfCurve(detectScores, lbs,'AMOC-mean');
            else
                error('m_tstSegSVM.m: unknown amocOpt');
            end;
            
            idx = find(xamoc <= targetFPR, 1);
            targetThresh = threshVals(idx);
            
            fireFrames = zeros(1, nTst);
            for i=1:nTst
                ff = find(detectScores{i} >= targetThresh, 1);
                if ~isempty(ff)
                    fireFrames(i) = ff;
                end;
            end;            
        end
    end    
end

