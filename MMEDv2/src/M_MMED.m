classdef M_MMED
% Reimplementation of MMED, purely in Matlab
% By: Minh Hoai Nguyen (minhhoai@cs.stonybrook.edu)
% Created: 18-Feb-2016
% Last modified: 1-Mar-2016
    
    methods (Static)
        % Ds: 1*n cell structure, Ds{i} is d*n_i matrix for the i^th time series
        % gtEvs: 2*n matrix for the location of the events in the signals
        % C: tradeoff param
        function [w, b, xi, lbObjVals, ubObjVals] = train(Ds, gtEvs, C, muOpts, featOpts, peOpts, mvOpts, methodOpt)
            nMaxIter = 100; % generally, the number of iterations should < 100            
            gapTol = 1e-3; % terminate if gap < gapTol*objVal.
            
            % removing inactive constraints can get stuck at saddle points
            % only remove inactive constraints if there is significant progress defined by this
            % tolerance value
            inactiveTol = 1e-3; 
            
            % number of violated constraints per time series. 
            % higher number leads to bigger quadratic program at each iteration
            % but the number of iterations will be smaller. Increase this if QP takes less time to
            % solve and negative mining take long time.
            nConstrPerTS = 10;  
            
            % Removing inactive constraints can slow the progress
            % Only remove inactive constrains when the total number of constraints exceed this 
            nMinConstraint = 2000; 
            
            featFunc = featOpts.func;
            n = length(Ds);
            k = featOpts.dim;
            k1 = k + 1;
            k1n = k + 1 + n;            
            
            % Add to objective: 0.5*w'*w
            H = zeros(k1n);
            linIdxs = sub2ind([k1n, k1n], 1:k, 1:k);
            H(linIdxs) = 1;                                     
            f = [zeros(k1,1); (C/n)*ones(n,1)];
            
            % Add lower bounds for xi_i >= 0
            lb = [-inf(k1,1); zeros(n,1)];
            ub = inf(k1n,1);
                                    
            w = zeros(k, 1);            
            b = 0;
            xi = zeros(n,1);
            
            ubObjVals = zeros(nMaxIter, 1);
            lbObjVals = zeros(nMaxIter, 1);
                                    
            minUbObjVal = inf;
            qpOpts = optimoptions('quadprog');
            qpOpts.Display = 'off';
            
            Aineq = [];
            bineq = [];
            for iter=1:nMaxIter
                fprintf('\n%s iter: %d\n', methodOpt, iter);
                
                extraVio = 0;
                newAineqs = cell(1, 2*n);
                newBineqs = zeros(2*n,1);
                nConstr = 0;

                startT = tic;
                for i=1:n
                    ml_progressBar(i, n, '  Hard negative mining', startT);
                    if strcmpi(methodOpt, 'mmed')
                        mvcs = M_MMED.findMVCs(w, b, Ds{i}, gtEvs(:,i), featFunc, mvOpts, peOpts, muOpts, nConstrPerTS);
                    elseif strcmpi(methodOpt, 'sosvm')
                        mvcs = M_MMED.findMVCs4SOSVM(w, b, Ds{i}, gtEvs(:,i), featFunc, mvOpts, nConstrPerTS);
                    else
                        error('unknown method option: %s', methodOpt);
                    end;
                    
                    if ~isempty(mvcs)
                        mvcSlack = mvcs(5,1); % slack of the most violated constraint

                        if xi(i) >= mvcSlack
                            continue;
                        else
                            extraVio = extraVio + mvcSlack - xi(i);                                                
                        end;        
                        
                        mvcs = mvcs(:, mvcs(5,:) > xi(i)); % only the ones incur a loss. 
                        
                        for u=1:size(mvcs,2)
                            nConstr = nConstr + 1;
                            cnstr_xi_coeff = zeros(n, 1);
                            cnstr_xi_coeff(i) = -1;
                            mvcType = mvcs(6,u);
                            mvcSeg = mvcs(1:2,u);
                            mvcPeSeg = mvcs(3:4,u);

                            if mvcType == 1
                                cnstr_w_coeff = featFunc(Ds{i}(:,mvcSeg(1):mvcSeg(2)));                        
                                cnstr_b_coeff = 1;                                                
                                newAineqs{nConstr} = [cnstr_w_coeff; cnstr_b_coeff; cnstr_xi_coeff]';
                                newBineqs(nConstr) = -1;                        
                            elseif mvcType == 2                        
                                peRatio = (mvcPeSeg(2) - mvcPeSeg(1) + 1)/(gtEvs(2,i) - gtEvs(1,i) + 1);
                                muVal = M_MMED.muFunc(muOpts.alpha, muOpts.beta, peRatio);                        
                                cnstr_w_coeff = -muVal*featFunc(Ds{i}(:,mvcPeSeg(1):mvcPeSeg(2)));          
                                cnstr_b_coeff = -muVal;                                                  
                                newAineqs{nConstr} = [cnstr_w_coeff; cnstr_b_coeff; cnstr_xi_coeff]';                        
                                newBineqs(nConstr) = -muVal;                        
                            elseif mvcType == 3                                              
                                cnstr_w_coeff = featFunc(Ds{i}(:,mvcSeg(1):mvcSeg(2))) - ...
                                                featFunc(Ds{i}(:,mvcPeSeg(1):mvcPeSeg(2)));

                                peRatio = (mvcPeSeg(2) - mvcPeSeg(1) + 1)/(gtEvs(2,i) - gtEvs(1,i) + 1);
                                muVal = M_MMED.muFunc(muOpts.alpha, muOpts.beta, peRatio);
                                deltaVal = M_MMED.cmpDelta(mvcPeSeg, mvcSeg);

                                cnstr_w_coeff = cnstr_w_coeff*muVal;            
                                cnstr_b_coeff = 0;                                                  
                                newAineqs{nConstr} = [cnstr_w_coeff; cnstr_b_coeff; cnstr_xi_coeff]';                        
                                newBineqs(nConstr) = -deltaVal*muVal;
                            end;     
                        end
                    end
                end;
                
                extraVio = C/n*extraVio;
                if iter > 1
                    curUbObjVal = curLbObjVal + extraVio;                    
                    ubObjVals(iter-1) = curUbObjVal;
                    minUbObjVal = min(minUbObjVal, curUbObjVal);                    
                    
                    objValGap = minUbObjVal - curLbObjVal;
                    fprintf('  curLbObjVal: %g, gap: %g\n', curLbObjVal, objValGap);
                    if objValGap < gapTol*curLbObjVal
                        break;
                    end;
                end;
                fprintf('  Number of new constraints: %d\n', nConstr);
                
                Aineq = cat(1, Aineq, newAineqs{1:nConstr});
                bineq = cat(1, bineq, newBineqs(1:nConstr));
                fprintf('  Solving QP with %d constraints\n', size(Aineq,1));
                %[qpSol, curLbObjVal] = cplexqp(H,f,Aineq,bineq,[],[],lb,ub);                
                [qpSol, curLbObjVal] = quadprog(H,f,Aineq,bineq,[],[],lb,ub, [], qpOpts);
                
                
                lbObjVals(iter) = curLbObjVal;                
                if iter > 1 && lbObjVals(iter) < lbObjVals(iter-1) - 0.1
                    keyboard;
                    error('Something is wrong, the objective value cannot decrease');
                end;
                
                w = qpSol(1:k);
                b = qpSol(k1);
                xi = qpSol(k1+1:end);
                
                % Remove untight constraints to reduce the size of the quadratic program
                % However, the procedure can get stuck into saddle point. 
                % So we don't remove inactive constraints if the objective values do not increase              
                if iter > 1 && lbObjVals(iter) - lbObjVals(iter-1) > inactiveTol ...
                        && size(Aineq, 1) > nMinConstraint
                    
                    tightness = bineq - Aineq*qpSol;
                    [sortedVals, sortIdxs] = sort(tightness, 'descend');
                    inactiveIdxs = sortIdxs(sortedVals > inactiveTol);
                    
                    max2remove =size(Aineq,1) - nMinConstraint;                    
                    inactiveIdxs = inactiveIdxs(1:min(length(inactiveIdxs),max2remove));

                    Aineq(inactiveIdxs, :) = [];
                    bineq(inactiveIdxs) = [];
                    fprintf('  Removing %d inactive constraints, remaining: %d\n', length(inactiveIdxs), size(Aineq,1));
                else
                    fprintf('  No inactive constraints are removed.\n');
                end; 
            end;
            nIter = iter - 1;
            lbObjVals = lbObjVals(1:nIter);
            ubObjVals = ubObjVals(1:nIter);            
        end
        
        % D: d*n matrix for a times series of length n
        % featFunc, detOpts: feature function and detection options. See m_params 
        % detEvs: 3*m matrix for detected events
        %   m is the total number of time steps where the detector is evaluated.
        %   m can be smaller than n if detOpts.swStep > 1. This corresponds to dropping frames. 
        %   detEvs(:,i) is [s, e, score] 
        %       where e is the i^th time step where the detector is evaluated.
        %       s is the start of the segment with highest score ENDING at e. 
        function detEvs = eval(w, b, D, featFunc, detOpts)
            n = size(D,2);
            
            evalTimes = fliplr(n:-detOpts.swStep:1);
            m = length(evalTimes);
            detEvs = zeros(3, m);
            for t=1:m
                e = evalTimes(t);
                if e <= detOpts.lmax % if i < lmax, always consider the segment from the beginning
                    feat = featFunc(D(:,1:e));
                    maxScore = feat'*w + b;
                    maxStart = 1; 
                else
                    maxScore = -inf;
                end;                
                lrange = detOpts.lmin:detOpts.lstep:min(detOpts.lmax, e-1);
                if ~isempty(lrange)
                    Feats = zeros(length(w), length(lrange));
                    for j=1:length(lrange)
                        l = lrange(j);
                        Feats(:,j) = featFunc(D(:,(e-l+1):e));
                    end;
                    scores = Feats'*w + b; 
                    [maxVal, maxIdx] = max(scores);
                    if maxVal > maxScore
                        maxScore = maxVal;
                        maxStart = e - lrange(maxIdx) + 1;
                    end;
                end;
                
                detEvs(:,t) = [maxStart, e, maxScore];
            end;            
        end;
        
        % Exactly the same as eval. It is a bit faster since we evaluate
        % multiple segments of the same length together. 
        function detEvs = eval_fast(w, b, D, featFunc, detOpts)
            n = size(D,2);            
            lrange = detOpts.lmin:detOpts.lstep:min(detOpts.lmax, n-1);
            A = -inf(n,n);
            
            for j=1:length(lrange)
                segLen = lrange(j);
                                                                
                startLocs = fliplr((n-segLen+1):-detOpts.swStep:1);
                endLocs   = fliplr(n:-detOpts.swStep:segLen);
                segData = zeros(size(D,1), segLen, length(startLocs));
                
                for t=1:length(startLocs);                    
                    segData(:,:,t) = D(:,startLocs(t):endLocs(t));
                end;
                                
                Feats = featFunc(segData);
                score = w'*Feats + b;
                linIdxs = sub2ind(size(A), startLocs, endLocs);
                A(linIdxs) = score;
            end;
            
            for i=1:min(n, detOpts.lmax)
                feat = featFunc(D(:,1:i));
                A(1,i) = feat'*w + b;
            end;
            [maxScore, rowIdxs] = max(A, [], 1);            
            detEvs = [rowIdxs; 1:n; maxScore];            
        end
        
        
        % Find most violated constraints
        % D: d*n matrix for a time series of n time steps
        % gtEv: 2*1 vector for the start and end of the ground truth event
        % w: k*1 weight vector for SVM
        % b: bias term of SVM
        % peOpts, mvOpts: see m_params.m
        % featFunc: see m_params
        %   The dimension of features vectors returned must match the length of w. 
        % nMaxConstr: maximum number of constraint to retrieve
        % Outputs:
        %   mvcs: 6*k: (k <= nMaxConstr) for k most violated constraints
        %       Non-maximum suppression is run to ensure the constraints are significantly different
        %       mvcs(:,i) is a 6-dim vector for [vioSeg_start; vioSeg_end; partialEvent_start;
        %                                        partilEven_end; mvcSlack; mvcType]
        %       mvcType: type of most violated constraint. There are three types of constraints
        %           Type1: before the event starts, all segments must have score <= -1
        %           Type2: after the event starts, partial events must have score >= 1
        %           Type3: partial event should have higher score than past segments
        %       mvcSlack, vioSeg, partialEvent: are slack variable, violated segment, 
        %           violated partial event of a violated constraint. 
        %       If mvcType = 1, partialEvent = [nan;nan];
        %       If mvcType = 2, vioSeg       = [nan;nan];
        %       If mvcType = 3, both partialEvent and vioSeg are not nan. 
        %   We only return constraints where mvcSlack are positive
        function mvcs = findMVCs(w, b, D, gtEv, featFunc, mvOpts, peOpts, muOpts, nMaxConstr)  
            % overlap thresholds for non-maximum suppression
            overlap1 = 0.6; % for violated segment
            overlap2 = 0.7; % for partial event
            
            % segments from sliding windows and associates scores
            [swScores, swSegs] = M_MMED.getSlideWinSegScores(w, b, D, featFunc, mvOpts);
            
            % If the event is empty
            if isnan(gtEv(1)) || gtEv(1) == 0 || gtEv(1) > gtEv(2)                                
                swSlacks = 1 + swScores;
                Aux0 = M_MMED.segNms([swSegs; swSlacks], overlap1, nMaxConstr);
                mvcs = zeros(6, size(Aux0,2));
                mvcs([1:2,5],:) = Aux0;
                mvcs(3:4,:) = nan;                 
                mvcs(6,:) = 1; % mvc type
                return;
            end;                        

            % Find MVC for Type1 constraints
            % Before the event start, all segment should have a score <= -1. 
            % The slack value is: 1 + score.             
            b4SegsIdxs = swSegs(2,:) < gtEv(1);
            if any(b4SegsIdxs)
                b4Segs = swSegs(:, b4SegsIdxs); % segments before the event start
                b4SegsSlacks = 1 + swScores(b4SegsIdxs);
                
                Aux1 = M_MMED.segNms([b4Segs; b4SegsSlacks], overlap1, nMaxConstr);
                mvc1 = zeros(6, size(Aux1,2));
                mvc1([1:2,5],:) = Aux1;
                mvc1(3:4,:) = nan;                 
                mvc1(6,:) = 1; % mvc type
            else
                mvc1 = [];
            end;

            % Find MVC for Type2 constraints
            gtEvLen = gtEv(2) - gtEv(1) + 1;
            peEnds = (gtEv(1) - 1) + fliplr(gtEvLen:-peOpts.lstep:peOpts.lmin); % end points of partial events considered            
            peSegs = zeros(2, length(peEnds));
            peSegs(1,:) = gtEv(1);
            peSegs(2,:) = peEnds; 
            
            k = length(w);
            peFeats = zeros(k, length(peEnds)); % feature vectors for partial events
            for i=1:length(peEnds)    
                peFeats(:,i) = featFunc(D(:,gtEv(1):peEnds(i)));
            end
            peScores = w'*peFeats + b;
            slackVals2 = 1 - peScores;
            peRatio = (peSegs(2,:) - peSegs(1,:) + 1)/(gtEv(2) - gtEv(1) + 1);
            slackVals2 = slackVals2.*M_MMED.muFunc(muOpts.alpha, muOpts.beta, peRatio);
            
            Aux2 = M_MMED.segNms([peSegs; slackVals2], overlap2, nMaxConstr);
            mvc2 = zeros(6, size(Aux2,2));
            mvc2(1:2,:) = nan;
            mvc2(3:5,:) = Aux2;
            mvc2(6,:) = 2;
                        
            % Find MVC for Type3 constraints
            % different between a sliding wind segment and a partial score
            scoreDiff = repmat(swScores, length(peScores), 1) - repmat(peScores(:), 1, length(swScores));
            
            % Loss between events and segments
            Delta = zeros(size(scoreDiff));
            for i=1:length(peScores)
                Delta(i,:) = M_MMED.cmpDelta(peSegs(:,i), swSegs);
            end;
            slackVals3 = scoreDiff + Delta;            
            slackVals3 = slackVals3.*repmat(M_MMED.muFunc(muOpts.alpha, muOpts.beta, peRatio(:)), 1, size(slackVals3,2));
            
            % For a partial event, only need to have a constraint with a segment in the past
            invalidConstraintPair = false(size(scoreDiff));
            for i=1:(length(peScores)-1) % peSegs(:,end) is the full event
                invalidConstraintPair(i, peSegs(2,i) < swSegs(2,:)) = true;
            end;

            validIdxs = 1:numel(slackVals3);
            validIdxs(invalidConstraintPair(:)) = [];
            
            [rowIds, colIds] = ind2sub(size(slackVals3), validIdxs);            
            Aux3a = [swSegs(:, colIds); peSegs(:, rowIds); slackVals3(validIdxs)];
            Aux3b = M_MMED.segPairNms(Aux3a, overlap1, overlap2, nMaxConstr);
                        
            mvc3 = zeros(6, size(Aux3b,2));
            mvc3(1:5,:) = Aux3b;
            mvc3(6,:) = 3;
            
            mvcs = cat(2, mvc1, mvc2, mvc3);            
            [~, sortIdxs] = sort(mvcs(5,:), 'descend');
            sortIdxs = sortIdxs(1:min(nMaxConstr, length(sortIdxs)));
            mvcs = mvcs(:, sortIdxs);
        end        

        % Find most violated constraint for SOSVM
        function mvcs = findMVCs4SOSVM(w, b, D, gtEv, featFunc, mvOpts, nMaxConstr)
            % overlap thresholds for non-maximum suppression
            overlap1 = 0.6; % for violated segment
            
            % segments from sliding windows and associates scores
            [swScores, swSegs] = M_MMED.getSlideWinSegScores(w, b, D, featFunc, mvOpts);
            
            % If the event is empty
            if isnan(gtEv(1)) || gtEv(1) == 0 || gtEv(1) > gtEv(2)                                
                swSlacks = 1 + swScores;
                Aux0 = M_MMED.segNms([swSegs; swSlacks], overlap1, nMaxConstr);
                mvcs = zeros(6, size(Aux0,2));
                mvcs([1:2,5],:) = Aux0;
                mvcs(3:4,:) = nan;                 
                mvcs(6,:) = 1; % mvc type
                return;
            end;                        

            % Find MVC for Type1 constraints
            % Before the event start, all segment should have a score <= -1. 
            % The slack value is: 1 + score.             
            b4SegsIdxs = swSegs(2,:) < gtEv(1);
            if any(b4SegsIdxs)
                b4Segs = swSegs(:, b4SegsIdxs); % segments before the event start
                b4SegsSlacks = 1 + swScores(b4SegsIdxs);
                
                Aux1 = M_MMED.segNms([b4Segs; b4SegsSlacks], overlap1, nMaxConstr);
                mvc1 = zeros(6, size(Aux1,2));
                mvc1([1:2,5],:) = Aux1;
                mvc1(3:4,:) = nan;                 
                mvc1(6,:) = 1; % mvc type
            else
                mvc1 = [];
            end;
            
            % Find MVC for Type 2
            feFeat = featFunc(D(:,gtEv(1):gtEv(2))); % feature of full event
            feScore = w'*feFeat + b;                                    
            mvc2Slack = 1 - feScore;
            if mvc2Slack > 0
                mvc2 = [nan; nan; gtEv; mvc2Slack; 2];
            else
                mvc2 = [];
            end;
                        
            % Find most violated constraint compared with the full event             
            % different between a sliding win segment and the score of a full event
            scoreDiff = swScores - feScore; 
            
            % Loss between events and segments
            Delta = M_MMED.cmpDelta(gtEv, swSegs);            
            slackVals = scoreDiff + Delta;
                                    
            Aux3 = M_MMED.segNms([swSegs; slackVals], overlap1, nMaxConstr);
            mvc3 = zeros(6, size(Aux3,2));
            mvc3([1,2,5],:) = Aux3;
            mvc3(3:4,:) = repmat(gtEv, 1, size(Aux3,2));
            mvc3(6,:) = 3;
            
            mvcs = cat(2, mvc1, mvc2, mvc3);
            [~, sortIdxs] = sort(mvcs(5,:), 'descend');
            sortIdxs = sortIdxs(1:min(nMaxConstr, length(sortIdxs)));
            mvcs = mvcs(:, sortIdxs);
        end
                        
        % Mu function, piece-wise linear
        % x: k*1 vector for k values to be evaluate
        function mu = muFunc(alpha, beta, x)
            mu = zeros(size(x));
            mu(x == 0) = 1;
            mu(x<=alpha) = 0;
            mu(x>= beta) = 1;
            idxs = and(x > alpha, x < beta);
            mu(idxs)= (x(idxs) - alpha)/(beta - alpha);            
        end;
        
        % Compute the Delta loss between an event and a set of segments
        % The delta loss is 1 - overlap (intersection/union)
        % event: 2*1 vector for the start and end of an event
        % segs: 2*n matrix for the start and end of n segments        
        function delta = cmpDelta(event, segs)
            u = segs(2,:) - event(1) + 1;
            v = event(2) - segs(1,:) + 1;
            
            % if either u or v is not positive, the overlap is 0
            % other wise the overlap is min(u,v)/max(u,v)            
            overlap = zeros(1, size(segs,2));
            idxs = and(u >=1, v >= 1);
            overlap(idxs) = min(u(idxs), v(idxs))./max(u(idxs), v(idxs));
            delta = 1 - overlap;
        end;        
        
        % Get sliding window segments and scores
        % swOpts: see m_params
        % w, b: weight vector and bias for SVM
        % featFunc: feature function
        %   Expected input: signalDim*segLen*nSeg matrix. This is a data matrix for nSeg segments.
        %       Each segment has the length segLen, and the raw data dimension is signalDim
        %   Return output: k*nSeg 2D matrix, each column is a feature vector for a segment.
        %       k is the dimension of feature vectors, which should equal to the length of w. 
        function [svmScores, segs] = getSlideWinSegScores(w, b, D, featFunc, swOpts) 
            lmax = swOpts.lmax;
            if isinf(lmax)
                lmax = size(D,2);
            end;
            lmin = swOpts.lmin;
            lstep = swOpts.lstep;
            swStep = swOpts.swStep;
            
            winLens = fliplr(lmax:-lstep:lmin);                        
            [svmScores, segs] = deal(cell(1, length(winLens)));
            for i=1:length(winLens)
                winLen = winLens(i);                
                swObj = ML_SlideWin1D(D, winLen, swStep, 500, 1);                                
                nBatch = swObj.getNBatch();                
                [svmScores_i, startLocs_i] = deal(cell(1, nBatch));
                for batchId=1:nBatch;
                    [batchD, startLocs_i{batchId}] = swObj.getBatch(batchId);                    
                    feats = featFunc(batchD);
                    svmScores_i{batchId} = w'*feats + b;
                end;
                svmScores{i} = cat(2, svmScores_i{:});
                startLocs_i = cat(2, startLocs_i{:});
                segs{i} = [startLocs_i; startLocs_i + winLen - 1];
            end;
            segs = cat(2, segs{:});
            svmScores = cat(2, svmScores{:});
        end;
        
        
        % scoreSegs: 3*n matrix, scoreSegs(:,i) is [start, end, score]
        function top = segNms(scoreSegs, overlap, maxRetrieval)
            scoreSegs = scoreSegs(:, scoreSegs(3,:) > 0);
            if isempty(scoreSegs)
                top = [];
            else
                s = scoreSegs(1,:); 
                e = scoreSegs(2,:); 
                score = scoreSegs(3,:);
                
                [~, I] = sort(score);
                pick = zeros(1, size(scoreSegs,2));
                cnt = 0;                
                while ~isempty(I)
                    last = length(I);
                    i = I(last);
                    cnt = cnt + 1;                    
                    pick(cnt) = i;
                    if cnt >= maxRetrieval
                        break;
                    end;
                    
                    suppress = false(1, last);
                    suppress(last) = true;
                    
                    maxs = max(s(i), s(I(1:last-1)));
                    mins = min(s(i), s(I(1:last-1)));
                    
                    maxe = max(e(i), e(I(1:last-1)));
                    mine = min(e(i), e(I(1:last-1)));
                    
                    inter = mine - maxs + 1;
                    union = maxe - mins + 1;
                    suppress(1:last-1) = inter./union > overlap;                    
                    I(suppress) = [];
                end
                pick = pick(1:cnt);
                top = scoreSegs(:, pick);
                
            end   
        end
            
        % segPairs: 5*n matrix, segPair(:,i) is [seg1Start, seg2Start, seg2Start, seg2End, score]
        % overlap1, overlap2
        % maxRetrieval: maximum number to retrieve
        function top = segPairNms(segPairs, overlap1, overlap2, maxRetrieval)
            segPairs = segPairs(:, segPairs(5,:) > 0);
            
            if isempty(segPairs)
                top = [];
            else
                s1 = segPairs(1,:); 
                e1 = segPairs(2,:); 
                s2 = segPairs(3,:);
                e2 = segPairs(4,:);
                score = segPairs(5,:);
                
                [~, I] = sort(score);
                pick = zeros(1, size(segPairs,2));
                cnt = 0;                
                while ~isempty(I)
                    last = length(I);
                    i = I(last);
                    cnt = cnt + 1;
                    pick(cnt) = i;
                    if cnt >= maxRetrieval
                        break;
                    end;
                    
                    suppress = false(1, last);
                    suppress(last) = true;
                    
                    maxs1 = max(s1(i), s1(I(1:last-1)));
                    mins1 = min(s1(i), s1(I(1:last-1)));                    
                    maxe1 = max(e1(i), e1(I(1:last-1)));
                    mine1 = min(e1(i), e1(I(1:last-1)));
                    
                    maxs2 = max(s2(i), s2(I(1:last-1)));
                    mins2 = min(s2(i), s2(I(1:last-1)));                    
                    maxe2 = max(e2(i), e2(I(1:last-1)));
                    mine2 = min(e2(i), e2(I(1:last-1)));
                                        
                    inter1 = mine1 - maxs1 + 1;
                    union1 = maxe1 - mins1 + 1;                    
                    inter2 = mine2 - maxs2 + 1;
                    union2 = maxe2 - mins2 + 1;
                    
                    suppress(1:last-1) = and(inter1./union1 > overlap1, inter2./union2 > overlap2);                    
                    I(suppress) = [];
                end
                pick = pick(1:cnt);
                top = segPairs(:, pick);                
            end  
        end

                
        % segData: d*segLen*nSeg matrix. This is a data matrix for nSeg segments.
        %       Each segment has the length segLen, and the raw data dimension is d
        % Feats: d*nSeg 2D matrix, each column is a feature vector for a segment.
        function Feats = sumL1Norm(segData)
            Aux = mean(segData, 2);
            Aux = permute(Aux, [1, 3, 2]);
            Feats = Aux./repmat(sum(Aux,1), [size(Aux,1), 1]);            
        end;
        
        % Unnormalized Histogram features
        % segData: 1*segLen*nSeg matrix. This is a data matrix for nSeg segments.
        %   Each segment has the length segLen, and the raw data dimension is 1
        % histBins: 1*k vector for centers of histogram bins. 
        %   E.g, if raw sensor values are from 1 to k, then we can use histBins=1:k
        % Feats: k*nSeg 2D matrix, each column is a feature vector for a segment.
        function Feats = hist(segData, histBins)
            Aux = permute(segData, [2, 3, 1]);
            if size(Aux, 1) == 1
                Aux = repmat(Aux, [2, 1]); % to prevent it becomes a row vector
                Feats = hist(Aux, histBins);
            else
                Feats = hist(Aux, histBins);                
            end
            if size(Feats,1) == 1
                Feats = Feats(:);
            end
        end;
        
        % feature is the difference between the last frame and the end frame
        function Feats = endDiff(segData)
            Aux = segData(:,end,:) - segData(:,1,:);
            Feats = permute(Aux, [1,3,2]);            
        end;
        
        % To compute feature vector for a time series segment,
        % we divide it into multiple intervals, take the mean for each interval and 
        % concatenate the feature vectors for the intervals
        % segData: d*segLen*nSeg matrix. This is a data matrix for nSeg segments.
        %       Each segment has the length segLen, and the raw data dimension is d
        function Feats = meanDivs(segData, nDiv)
            [d, segLen,nSeg] = size(segData);
            
            q = floor(segLen/nDiv);
            r = segLen - nDiv*q;            
            s1 = 1 + (0:(r-1))*(q+1);
            e1 = (1:r)*(q+1);
            s2 = 1+r+(r:nDiv-1)*q;
            e2 = r + ((r+1):nDiv)*q;
            s = cat(2, s1, s2);
            e = cat(2, e1, e2);            
            s = min(s, e); % handle case segLen < nDiv
                                    
            Feats = zeros(d*nDiv, nSeg);
            for i=1:nDiv
                Aux = mean(segData(:, s(i):e(i),:),2);
                Feats((1+(i-1)*d):(i*d),:) = Aux; %squeeze(Aux);
            end;
        end;
        
        % To compute feature vector for a time series segment,
        % we divide it into multiple intervals, take the sum for each interval and 
        % concatenate the feature vectors for the intervals
        % segData: d*segLen*nSeg matrix. This is a data matrix for nSeg segments.
        %       Each segment has the length segLen, and the raw data dimension is d
        function Feats = sumDivs(segData, nDiv)
            [d, segLen,nSeg] = size(segData);
            
            q = floor(segLen/nDiv);
            r = segLen - nDiv*q;            
            s1 = 1 + (0:(r-1))*(q+1);
            e1 = (1:r)*(q+1);
            s2 = 1+r+(r:nDiv-1)*q;
            e2 = r + ((r+1):nDiv)*q;
            s = cat(2, s1, s2);
            e = cat(2, e1, e2);            
            s = min(s, e); % handle case segLen < nDiv
                                    
            Feats = zeros(d*nDiv, nSeg);
            for i=1:nDiv
                Aux = sum(segData(:, s(i):e(i),:),2);
                Feats((1+(i-1)*d):(i*d),:) = Aux; %squeeze(Aux);
            end;
        end;
    end
end
