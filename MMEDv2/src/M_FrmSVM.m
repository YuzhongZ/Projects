classdef M_FrmSVM
    % Frame-based SVM
    % By: Minh Hoai Nguyen (minhhoai@cs.stonybrook.edu)
    % Created: 25-Feb-2016
    % Last modified: 25-Feb-2016
    
    methods (Static)
        % Train a frame-based SVM
        % Inputs:
        %   Ds: 1*n cell structure of time series, Ds{i} is d*n_i matrix
        %   lbs: 2*n matrix for gt labels, lbs(:,i) is 2*1 vector for event's onset and offset
        %   kOpt: kernel option
        %   C: C for SVM
        % Outputs:
        %   w, b:
        function [w, b] = train(Ds, lbs, C)
            maxPosSample = 2000; % maximum number of positive samples
            maxNegSample = 2000; % maximum number of negative samples
            
            n = length(Ds);
            posFrms = cell(1, n);
            negFrms = cell(1, n);
            for i=1:n
                D = Ds{i};
                lb = lbs(:,i);
                if lb(1) > 0 % contain the event of interest
                    posFrms{i} = D(:, lb(1):lb(2));
                end;
                negFrms{i} = D(:, [1:lb(1)-1, lb(2)+1:size(D,2)]);
            end;
            
            posD = cat(2, posFrms{:});
            negD = cat(2, negFrms{:});
            nPos = size(posD, 2);
            nNeg = size(negD, 2);
            
            if nNeg > maxNegSample
                k = floor(nNeg/maxNegSample); % sampling rate
                negD = negD(:,1:k:end);
            end;
            if nPos > maxPosSample
                k = floor(nPos/maxPosSample);
                posD = posD(:,1:k:end);
            end;
            nPos = size(posD, 2);
            nNeg = size(negD, 2);
                        
            trLb = [ones(nPos,1); -ones(nNeg, 1)];
            TrD = [posD, negD];
            
            % Include option for reweighting two classes
            opts = sprintf('-t 0 -c %g -w1 %g -w-1 %g', C, 1/nPos, 1/nNeg);
            % opts = sprintf('-t 0 -c %g', C);
            model = svmtrain(trLb, TrD', opts); %#ok<SVMTRAIN> Must use LibSVM
            w = model.SVs'*model.sv_coef;
            b = - model.rho;            
        end
        
        % Inputs:
        %   Ds: n*1 cell structure of time series, Ds{i} is d*n_i matrix
        %   lbs: 2*n matrix for gt labels, lbs(:,i) is 2*1 vector for event's onset and offset
        %   kOpt: kernel option
        %   w, b: SVM parameters
        %   amocOpt: 'AMOC-mean' or 'AMOC-median'
        % Outputs:
        %   rocArea:
        %   xroc, yroc, xamoc, yamoc: performance curves
        function [rocArea, xamoc, yamoc] = test(Ds, lbs, w, b, amocOpt)
            n = length(Ds);
            detectScores = cell(1,n);
            for i=1:n
                decVals = w'*Ds{i} + b;
                detectScores{i} = cummax(decVals);
            end;
            
            [~, ~, rocArea]   = m_getPerfCurve(detectScores, lbs,'ROC');
            if ~exist('amocOpt', 'var') || isempty(amocOpt) || strcmpi(amocOpt, 'AMOC-median')
                [xamoc, yamoc] = m_getPerfCurve(detectScores, lbs,'AMOC');
            elseif strcmpi(amocOpt, 'AMOC-mean')
                [xamoc, yamoc] = m_getPerfCurve(detectScores, lbs,'AMOC-mean');
            else
                error('m_tstFrmSVM.m: unknown amocOpt');
            end;
        end
        
        % Find the frames where the dectector fires at a target false positive rate.
        % Inputs:
        %   Ds: n*1 cell structure of time series, Ds{i} is d*n_i matrix
        %   lbs: 2*n matrix for gt labels, lbs(:,i) is 2*1 vector for event's onset and offset
        %   kOpt: kernel option
        %   w, b: SVM parameters
        %   amocOpt: 'AMOC-mean' or 'AMOC-median'
        % Outputs:
        %   fireFrames: the frame numbers at which the detector fires
        function fireFrames = fireFrame_at_FPR(Ds, lbs, w, b, amocOpt, targetFPR)
            n = length(Ds);
            detectScores = cell(1,n);
            for i=1:n
                D_i = Ds{i};                 
                n_i = size(D_i, 2);
                decVals = w'*D_i + b;
                decVals = decVals(:)';
                dScr = decVals; % must be a row vector
                for j=2:n_i
                    dScr(j) = max(dScr(j), dScr(j-1));
                end;
                detectScores{i} = dScr;
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
            
            fireFrames = zeros(1, n);
            for i=1:n
                ff = find(detectScores{i} >= targetThresh, 1);
                if ~isempty(ff)
                    fireFrames(i) = ff;
                end;
            end;            
        end        
    end
end
