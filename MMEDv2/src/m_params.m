% Signatures of common parameters used in the code 
% This file is created to avoid having to explain a certain data structure multiple times
% By: Minh Hoai Nguyen (minhhoai@cs.stonybrook.edu)
% Created: 25-Feb-2016
% Last modified: 25-Feb-2016


% peOpts: Options to limit the partial events considered. This speed ups MMED training.
%   lmin: min length of partial events considered
%   lstep: length step
%       if lstep > 1, we don't consider all posible lens b/t lmin and the event length.
%       We only consider lengths: evLen:-lstep:lmin

% mvOpts: Options to limit the number of segments considered while searching for the
%       most violated constraints. This speed ups the training of MMED and SOSVM.
%   lmin: min length of violated constraint
%   lmax: max length of violated constraints
%   lstep: length step
%       if lstep > 1, we don't consider all posible lengths between lmin and lmax.
%       We only consider lengths: lmax:-lstep:lmin
%   swStep: sliding window step
%       if swStep > 1, we don't consider all posible ending points
%       We only consider end points: n:-swStep:1


% detOpts: Options for detection and evaluation. Use this to imit the number of segments considered 
%       while performing event detection at test time. At time t, to determine how likely the current 
%       time correspond to the event, we need to consider the past segments ending at t.
%   lmin: min length a past segment (lmin=1 correspond to a single frame at t)
%   lmax: max length a past segment
%   lstep: length step
%       if lstep > 1, we don't consider all posible lengths between lmin and lmax.
%       We only consider lengths: lmin:lstep:lmax
%   swStep: sliding window step
%       Normally, for detection, we would consider all time steps. 
%       If swStep > 1, we don't consider all positive time steps
%       We only consider time steps: n:-swStep:1


% featOpts: structure to specify the function to compute feature vectors of multiple time series
%       segments
%   dim: the dimension of the feature vectors 
%   func: is a function to compute feature vectors. The feature dimension must match dim. 
%       See featFunc for input output

% featFunc: a function that take a single 3D array and return a 2D matrix
% Input: 
%   segData: d*segLen*nSeg matrix. This is a data matrix for nSeg segments.
%       Each segment has the length segLen, and the raw data dimension is d
% Output:
%   Feats: k*nSeg matrix, each column is a feature vector for a time series segment
% Eg., @(x) M_MMED.sumL1Norm(x)
%      @(x) M_MMED.hist(x, 1:5)
%      @(x) M_MMED.endDiff(x)
