function m_main_asl(nRun)
% Experiment on Australian Sign Language
% This run HMM-based and SVM-based detectors and plot the results.
% Fully tested, fully working code
% By: Minh Hoai Nguyen (minhhoai@cmu.edu)
% Date: 10 May 2011
% Last modified: 1 Mar 2016

%addpath(genpath('/Users/hoai/Study/Libs/HMMall/')); % HMM implementation

addpath(genpath('../KMCode/'));
load ../data/ASL.mat D;
svmSaveFilePrefix = './ASL';

woiIDs = [2, 45, 94]; % This corresponds to: I love you.
TrD  = D(:, 1:15);   % first 15 examples of each signs are used for training
TstD = D(:, 16:27);   % last 12 eampls of each signs are used for testing.
nTrSeq  = 100; % no of training sequences
nTstSeq = 200; % no of testing sequences
bgSz_bf = 15;  % no of background words before the event of interest
bgSz_af = 5;  % no of background words after the event of itnerest
[trWordSeqs, trLbs]   = crtWordSeqs(TrD,  woiIDs, nTrSeq,  bgSz_bf, bgSz_af);
[tstWordSeqs, tstLbs] = crtWordSeqs(TstD, woiIDs, nTstSeq, bgSz_bf, bgSz_af);

if ~exist('nRun', 'var') 
    nRun = 2; % set number of runs, 10 is a good number for reliable average result
end

for runID=1:nRun
    startT = tic;
    fprintf('------------------------->Run %d\n', runID);
    % run SVM-based detectors and save results
    svmSaveFile = sprintf('%s_%02d.mat', svmSaveFilePrefix, runID);
    run(trWordSeqs, trLbs, tstWordSeqs, tstLbs, svmSaveFile);        
    fprintf('Run %d took %g\n', runID, toc(startT));
end;

% aggregate results and plot
aggregateRslt(nRun, svmSaveFilePrefix);
       
    
% do experiment for SVM-based detector
% trSens: a cell structure of sentences (or sequences) of interest
% trWordSeqs: training sequences
% tstWordSeqs: testing sequences
function run(trWordSeqs, trLbs, tstWordSeqs, tstLbs, saveFile)
    nState = 20;
    C = 1000;

    % Build Gaussian Mixture models from frames inside the true events
    trSens = cell(1, length(trWordSeqs)); % sentence of interest for training
    for k=1:length(trWordSeqs)
        trSens{k} = trWordSeqs{k}(:, trLbs(1,k):trLbs(2,k));
    end;
    trSens = cat(2, trSens{:});
    [mu_gmm, Sigma_gmm, mixmat_gmm] = mixgauss_em(trSens(:,1:5:end), nState);
    
    tr  = crtTSData(trWordSeqs, trLbs, mu_gmm, Sigma_gmm, mixmat_gmm);
    tst = crtTSData(tstWordSeqs, tstLbs, mu_gmm, Sigma_gmm, mixmat_gmm);
    d = size(tr.Ds{1},1);
    featOpts.dim = 2*d;
    featOpts.func = @(x) M_MMED.sumDivs(x, 2);
    
    % for training
    peOpts.lmin = 20;
    peOpts.lstep = 5;
    mvOpts.lmin = 20;
    mvOpts.lmax = 200;
    mvOpts.lstep = 5;
    mvOpts.swStep = 5;
    %muOpts.alpha = 0.25;
    muOpts.alpha = 0.1;
    muOpts.beta = 1;

    % for detection evaluation
    detOpts.lmin = 20;
    detOpts.lmax = 200;
    detOpts.lstep = 5;
    detOpts.swStep = 5;

    figure; fntSz = 16;lnWdth = 3;
    cnt = 0;
    [w_trunc, b_trunc] = M_SegSVM.train(tr.Ds, tr.segLbs, featOpts, C, [0.5, 1], mvOpts, 10);
    [rocArea_trunc, xamoc_trunc, yamoc_trunc] = ...
        M_SegSVM.test(tst.Ds, tst.segLbs, featOpts.func, detOpts, w_trunc, b_trunc, 'AMOC-mean');        
    fprintf('rocArea_Seg-[0.5,1]: %g\n', rocArea_trunc);    
    plot(xamoc_trunc, yamoc_trunc, 'k', 'LineWidth', lnWdth, 'LineStyle','-.'); hold on;
    drawnow;
    cnt = cnt+1; legends{cnt} = sprintf('Seg-[0.5,1] %.2f', 100*rocArea_trunc);
    
    startT = tic;
    [w_so, b_so] = M_MMED.train(tr.Ds, tr.segLbs, C, muOpts, featOpts, peOpts, mvOpts, 'SOSVM');    
    fprintf('training SOSVM took %g\n', toc(startT));    
    [rocArea_so, xamoc_so, yamoc_so] = ...
        M_SegSVM.test(tst.Ds, tst.segLbs, featOpts.func, detOpts, w_so, b_so, 'AMOC-mean');
    fprintf('rocArea_SOSVM: %g\n', rocArea_so);
    plot(xamoc_so, yamoc_so, 'b', 'LineWidth', lnWdth, 'LineStyle','--'); drawnow;
    cnt = cnt+1; legends{cnt} = sprintf('SOSVM %.2f', 100*rocArea_so);

    startT = tic;
    [w_mmed, b_mmed] = M_MMED.train(tr.Ds, tr.segLbs, C, muOpts, featOpts, peOpts, mvOpts, 'MMED');
    fprintf('training MMED took %g\n', toc(startT));
    [rocArea_mmed, xamoc_mmed, yamoc_mmed] = ...
        M_SegSVM.test(tst.Ds, tst.segLbs, featOpts.func, detOpts, w_mmed, b_mmed, 'AMOC-mean');
    fprintf('rocArea_MMED: %g\n', rocArea_mmed);
    plot(xamoc_mmed, yamoc_mmed, 'r', 'LineWidth', lnWdth); drawnow;
    cnt = cnt+1; legends{cnt} = sprintf('MMED %.2f', 100*rocArea_mmed);

    legend(legends{:}); axis([0, 1, 0, 1]); title('AMOC curves with Area under ROC');
    fprintf('ROC-area, Seg-[0.5,1]: %g, SOSVM: %g, MMED: %g\n', ...
        rocArea_trunc, rocArea_so, rocArea_mmed);
        
    xlabel('False Positive Rate', 'FontSize', fntSz); 
    ylabel('Normalized Time to Detect', 'FontSize', fntSz);
    title('Result for a single run');
    set(gca, 'FontSize', fntSz);
    drawnow;

    clear trWordSeqs tstWordSeqs tr tst mu_gmm Sigma_gmm mixmat_gmm nState;
    save(saveFile);    

% Aggreate results of different runs and plot results    
function aggregateRslt(nRun, svmSaveFilePrefix)
    yamoc_seg = cell(1, nRun);
    yamoc_so = yamoc_seg;
    yamoc_trunc = yamoc_seg;
    yamoc_mmed = yamoc_seg;
    
    rocArea_seg = zeros(1, nRun);
    rocArea_trunc = rocArea_seg;
    rocArea_so = rocArea_seg;
    rocArea_mmed = rocArea_seg;

    for runID=1:nRun
        rslt = load(sprintf('%s_%02d.mat', svmSaveFilePrefix, runID));

        yamoc_trunc{runID} = rslt.yamoc_trunc;
        yamoc_so{runID} = rslt.yamoc_so;
        yamoc_mmed{runID} = rslt.yamoc_mmed;
                
        rocArea_trunc(runID) = rslt.rocArea_trunc;
        rocArea_so(runID) = rslt.rocArea_so;
        rocArea_mmed(runID) = rslt.rocArea_mmed;                
        xamoc = rslt.xamoc_mmed;
    end;

    yamoc_trunc = mean(cat(2,yamoc_trunc{:}), 2);
    yamoc_so = mean(cat(2,yamoc_so{:}), 2);
    yamoc_mmed = mean(cat(2,yamoc_mmed{:}), 2);
    
    rocArea_trunc = mean(rocArea_trunc);
    rocArea_so = mean(rocArea_so);
    rocArea_mmed = mean(rocArea_mmed);

    lnWdth = 3; fntSz = 16;
    figure;   
    plot(xamoc, yamoc_trunc, 'k', 'LineWidth', lnWdth, 'LineStyle','-.'); hold on;
    plot(xamoc, yamoc_so, 'b', 'LineWidth',lnWdth, 'LineStyle','--');
    plot(xamoc, yamoc_mmed, 'r', 'LineWidth',lnWdth, 'LineStyle','-');
    legend('Seg-[0.5,1]', 'SOSVM', 'MMED', 'Location', 'NorthEast');
    xlabel('False Positive Rate', 'FontSize', fntSz); 
    ylabel('Normalized Time to Detect', 'FontSize', fntSz);
    title('Combined result for several runs');
    set(gca, 'FontSize', fntSz);
    axis square;
    
    fprintf('ROC-area, Seg-[0.5,1]: %g, SOSVM: %g, MMED: %g\n', ...
        rocArea_trunc, rocArea_so, rocArea_mmed);


% Create word sequences that contains a sentence of interest.    
% D{i,j}: for symbol i, session j.    
% woiIDs: IDs for the words to creat sentence of interest
% nSeq: number of sequence to generate
% bgSz: number of words in the backgrounds.
function [wordSeqs, lbs] = crtWordSeqs(D, woiIDs, nSeq, bgSz_bf, bgSz_af)
    [~, nSample] = size(D);
    wordSeqs = cell(1, nSeq);
    lbs = zeros(2, nSeq);
    D_fg = D(woiIDs, :); % foreground parts
    m = length(woiIDs); % length of the sentence of interest
    fgColIdxs = randi(nSample, [nSeq m]); % random sample IDs correspond to rows in woiIDs
    
    bgColIdxs_bf = randi(nSample, [nSeq bgSz_bf]);
    bgRowIdxs_bf = randi(size(D,1), [nSeq bgSz_bf]);

    bgColIdxs_af = randi(nSample, [nSeq bgSz_af]);
    bgRowIdxs_af = randi(size(D,1), [nSeq bgSz_af]);
    
    for k=1:nSeq
        fgIdxs = sub2ind(size(D_fg), 1:m, fgColIdxs(k,:));
        soi = cat(2, D_fg{fgIdxs});
        bgIdxs_bf = sub2ind(size(D), bgRowIdxs_bf(k,:), bgColIdxs_bf(k,:));
        bg_bf = cat(2, D{bgIdxs_bf});
        
        bgIdxs_af = sub2ind(size(D), bgRowIdxs_af(k,:), bgColIdxs_af(k,:));
        bg_af = cat(2, D{bgIdxs_af});
        
        lbs(:,k) = [size(bg_bf,2)+1, size(bg_bf,2) + size(soi, 2)];
        wordSeqs{k} = cat(2, bg_bf, soi, bg_af);
    end;

% Create time series data for SVM-based detectors
% From raw features, do soft-clustering using GMM, compute log-likelihood, retain
% the top three values and output it as frame-level feature vectors.
function tr = crtTSData(wordSeqs, lbs, mu, Sigma, mixmat)
    n = length(wordSeqs);
    tr.mus = cell(1, n);
    tr.Ds  = cell(1, n);
    tr.segLbs = lbs;
    
    d = length(mixmat);
    for i=1:n
        n_i = size(wordSeqs{i}, 2);        
        tr.Ds{i} = zeros(d, n_i);
        obslik = mixgauss_prob(wordSeqs{i}, mu, Sigma, mixmat);
        tr.Ds{i} = log(obslik);        
    end;

    