function [xvals, yvals, rocArea, threshVals] = m_getPerfCurve(detectScores, lbs, option)
% Get performance curve for early detection. This performance measure is unique
% for fast detection. 
% Inputs:
%   detectScores: 1*n cell structure for detection scores.
%       detectScores{i} is 1*n_i vector detection scores of time series i.
%       detectStores{i}(j) tells us the score of an event occurs from frame 1 to j of time
%       series i.
%   lbs: 2*n matrix for the occurences of events
%       lbs(:,i) is 2*1 vector of the onset and offset of the event.
%   option: either 'AMOC' or 'ROC'
% Outputs:
%   xvals, yvals: corresponding values for the performance curve.
%   rocArea: this value is only reliable if option is 'ROC'
%   threshVals: the threshold values for detection scores
%       threshVals(i) is responsible to the point at xvals(i) and yvals(i).
% By: Minh Hoai Nguyen (minhhoai@gmail.com)
% Date: 26 Jan 2011

for i=1:length(detectScores)
    detectScores{i} = cummax(detectScores{i});
end;

if strcmpi(option, 'AMOC') || strcmpi(option, 'AMOC-median')
    [FPRs, medianNT2Ds,~, threshVals] = m_mexAMOC(detectScores, lbs);
    xvals = FPRs;
    yvals = medianNT2Ds;
    rocArea = 0;
elseif strcmpi(option, 'AMOC-mean')
    [FPRs, ~, meanNT2Ds, threshVals] = m_mexAMOC(detectScores, lbs);
    xvals = FPRs;
    yvals = meanNT2Ds;
    rocArea = 0;
elseif strcmpi(option, 'ROC')
    [FPRs, TPRs, rocArea] = getROC(detectScores, lbs);
    xvals = FPRs;
    yvals = TPRs;
else
    error('m_getPerfCurve: unknown option');
end;

function [FPRs, TPRs, rocArea] = getROC(detectScores, lbs) 
    n = length(detectScores);
    negExScore = zeros(n, 1);
    posExScore = zeros(n, 1);
    nNeg = 0;
    nPos = 0;
    for i=1:n
        if (lbs(1,i) > 1)
            nNeg = nNeg + 1;
            negExScore(nNeg) = detectScores{i}(lbs(1,i)-1); 
        elseif lbs(1,i) == 0
            nNeg = nNeg + 1;
            negExScore(nNeg) = detectScores{i}(end);                
        end
        if (lbs(1,i) > 0) && (lbs(1,i) <= lbs(2,i))
            nPos = nPos + 1;
            posExScore(nPos) = detectScores{i}(lbs(2,i)); % at the end of the event
        end;
    end;
    negExScore = negExScore(1:nNeg);
    posExScore = posExScore(1:nPos);
    [rocArea, ~, ~, ~, ~, FPRs, TPRs] = ml_roc(negExScore, posExScore, 0, 'ACC');
