classdef ML_SlideWin1D
% Implement efficient sliding window for time series.
% This is a special case of ML_SlideWin
% By: Minh Hoai Nguyen (minhhoai@cs.stonybrook.edu)
% Created: 28-Feb-2016
% Last modified: 28-Feb-2016

    properties (Access = private)   
        Data;
        winSz;
        stepSz;        
        nBatch;
        batchStartIdxs;
        batchEndIdxs;
    end
        
    methods
        % Data: d*n 2D matrix 
        % winSz: a scalar for the size of the temporal sliding window
        % stepSz: a scalar of positive integers for the step size
        % memLimit: memory limit to determine the number of batches 
        %   a number, 1 means 1MB, 2 means 2MB. Default 100MB
        % isEndAlign: 
        %   default 0. If isEndAlign, the last window will end at the end of the sequence
        %   If isEndAlign = 0, the first window will start at the begining of the sequence 
        %   The stepSz might not be devisible by the length so the last window might not contain
        %   the end of the sequence. This option allows us to
        %   specify our preference: (1) start from the beginning or (2) terminate at the end
        %   At least one condition will be satisfied, and sometimes both will be satisfied. 
        function obj = ML_SlideWin1D(Data, winSz, stepSz, memLimit, isEndAlign)
            % Possible start indexes for the sliding window
            if exist('isEndAlign', 'var') && isEndAlign
                startIdxs = fliplr((size(Data,2) - winSz+1):-stepSz:1);            
            else
                startIdxs = 1:stepSz:(size(Data,2) - winSz+1);
            end
            endIdxs = startIdxs + winSz - 1;

            if ~exist('memLimit', 'var') || isempty(memLimit)
                memLimit = 100;
            end;
            
            % maximum number of elmements for a matrix, assuming double precision (16Bytes)
            maxMatrixSize = memLimit*10^6/16;             
            nWinPerBatch =  floor(maxMatrixSize/(size(Data,1)*winSz));
            nBatch_ = ceil(length(startIdxs)/nWinPerBatch);
            
            obj.Data = Data;
            obj.winSz = winSz;
            obj.stepSz = stepSz;      
            obj.nBatch = nBatch_;
            obj.batchStartIdxs = cell(1, nBatch_);
            obj.batchEndIdxs   = cell(1, nBatch_);
                        
            idxss = ml_kFoldCV_Idxs(length(startIdxs), nBatch_, 0);
            for i=1:nBatch_
                obj.batchStartIdxs{i} = startIdxs(idxss{i});                
                obj.batchEndIdxs{i}   = endIdxs(idxss{i});                
            end            
        end
        
        % Get the number of batches
        % The number of batches is automatically determined, based on memory limit
        function nBatch = getNBatch(self)
            nBatch = self.nBatch;
        end;
    
        % Get the batch of windows 
        % batchId: ID of the batch
        % D: d*winSz*m matrix, d: dimension of time series, m: number of windows
        % startLocs: 1*m vector 
        %   startLocs(i) is the start index for i^th window        
        function [D, startLocs] = getBatch(self, batchId)
            startLocs = self.batchStartIdxs{batchId};
            endLocs   = self.batchEndIdxs{batchId};
            
            D = zeros(size(self.Data,1), self.winSz, length(startLocs));            
            for t=1:length(startLocs);
                D(:,:,t) = self.Data(:,startLocs(t):endLocs(t));
            end;
        end
    end    
end

