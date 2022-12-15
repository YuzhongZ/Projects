mex m_mexAMOC.cppfunction m_main_CK(nRun)
% Perform experiment on CK+ dataset
% This load CAPP (Canonical appearance feature) for frame-level feature vector
% Segment level feature vector is the diff of the end frame and the start frame.
% By: Minh Hoai Nguyen (minhhoai@gmail.com)
% Date: 20 May 2011
% Last modidified: 24 Feb 2016

CKfile = '../data/CK+/CK.mat';
if ~exist(CKfile, 'file')
    [Ds, lbs, nFrms, imDirs] = getData();
    save(sprintf('%s', CKfile), 'Ds', 'lbs', 'nFrms', 'imDirs'); 
else
    load(sprintf('%s', CKfile), 'Ds', 'lbs', 'nFrms', 'imDirs'); 
end

mu_alpha = 0.0;
mu_beta = 0.5;
C = 1000;

prefix = sprintf('./C-%g_alpha-%g_beta-%g', C, mu_alpha, mu_beta);

if ~exist('nRun', 'var')
    nRun = 10; % should set nRun = 10 for reliable result
end
for i=1:nRun
    saveFile = sprintf('%s_%02d.mat', prefix, i);
    startT = tic;
    run(Ds, lbs, nFrms, imDirs, mu_alpha, mu_beta, C, saveFile);
    fprintf('One experiment run took %g seconds\n', toc(startT));
end;

aggregateRslt(nRun, prefix);


% single run and save the results in saveFile        单次运行并将结果保存在saveFile中 
function run(Ds, lbs, nFrms, imDirs, mu_alpha, mu_beta, C, saveFile)
    nTr = 10; % number of training samples per class
    [tr, tst, tstImDirs, tstLbs] = preData(Ds, lbs, nFrms, imDirs, nTr);

    % AMOC option
    amocOpt = 'AMOC-mean';
    targetFPR = 0.1; % interested at 10% FPR.

    d = size(tr.Ds{1},1);
    figure(1); clf; 
    cnt = 0;

    EndFrms = zeros(d, length(tr.Ds));
    for i=1:length(tr.Ds)
        EndFrms(:,i) = tr.Ds{i}(:,end);
    end;
    trPosIdxs = tr.segLbs(1,:) == 1;
    TrPosFrms = EndFrms(:, trPosIdxs);
    TrNegFrms = EndFrms(:, ~trPosIdxs);

    nPos = size(TrPosFrms,2);
    nNeg = size(TrNegFrms,2);
    trLb = [ones(nPos,1); -ones(nNeg, 1)];
    TrFrms = [TrPosFrms, TrNegFrms];

%     % Include option for reweighting two classes
%     opts = sprintf('-t 0 -c %g -w1 %g -w-1 %g', C, 1/nPos, 1/nNeg);
%     model = svmtrain(trLb, TrFrms, opts);  %ok<SVMTRAIN> use LibSVM
%     w_frm = model.SVs'*model.sv_coef;
%     b_frm = - model.rho;
  % Include option for reweighting two classes
    opts = sprintf('-t 0 -c %g -w1 %g -w-1 %g', C, 1/nPos, 1/nNeg);
    model = svmtrain(trLb, TrFrms', opts);  %#ok<SVMTRAIN> use LibSVM
    w_frm = model.SVs'*model.sv_coef;
    b_frm = - model.rho;

    figure; lnWdth = 3;
    [rocArea_frm, xamoc_frm, yamoc_frm] = ...
        M_FrmSVM.test(tst.Ds, tst.segLbs, w_frm, b_frm, amocOpt);
    fprintf('rocArea_frm: %g\n', rocArea_frm);
    plot(xamoc_frm, yamoc_frm, 'k', 'LineWidth', lnWdth, 'LineStyle','-.'); hold on;
    cnt = cnt+1; legends{cnt} = sprintf('frm, ROCarea: %.3f', 100*rocArea_frm);

    [w_allfrm, b_allfrm] = M_FrmSVM.train(tr.Ds, tr.segLbs, C);
    [rocArea_allfrm, xamoc_allfrm, yamoc_allfrm] = ...
        M_FrmSVM.test(tst.Ds, tst.segLbs, w_allfrm, b_allfrm, amocOpt);
    fprintf('rocArea_allfrm: %g\n', rocArea_allfrm);
    plot(xamoc_allfrm, yamoc_allfrm, 'g', 'LineWidth', lnWdth, 'LineStyle','-.' ); hold on;
    cnt = cnt+1; legends{cnt} = sprintf('allfrm, ROCarea: %.3f', 100*rocArea_allfrm);
    
    muOpts.alpha = mu_alpha;
    muOpts.beta  = mu_beta;
    peOpts.lmin = 1;
    peOpts.lstep = 1;
    mvOpts.lmin = 1;
    mvOpts.lmax = inf;
    mvOpts.lstep = 1;
    mvOpts.swStep = 1;
    featOpts.dim = d;
    featOpts.func = @(x) M_MMED.endDiff(x);
    detOpts.lmin = 2;
    detOpts.lmax = inf;
    detOpts.lstep = 1;
    detOpts.swStep = 1;
    
    startT = tic;
    [w_so, b_so] = M_MMED.train(tr.Ds, tr.segLbs, C, muOpts, featOpts, peOpts, mvOpts, 'SOSVM');
    fprintf('training SOSVM took %g\n', toc(startT));
    
    [rocArea_so, xamoc_so, yamoc_so] = ...
        M_SegSVM.test(tst.Ds, tst.segLbs, featOpts.func, detOpts, w_so, b_so, amocOpt);
    fireFrames_so = M_SegSVM.fireFrame_at_FPR(tst.Ds, tst.segLbs, featOpts.func, detOpts, w_so, b_so, amocOpt, targetFPR);

    fprintf('rocArea_so: %g\n', rocArea_so);    
    plot(xamoc_so, yamoc_so, 'b', 'LineWidth', lnWdth, 'LineStyle','--'); hold on;
    cnt = cnt+1; legends{cnt} = sprintf('SOSVM, ROCarea: %.2f', 100*rocArea_so);

    startT = tic;
    [w_mmed, b_mmed] = M_MMED.train(tr.Ds, tr.segLbs, C, muOpts, featOpts, peOpts, mvOpts, 'mmed');
    fprintf('training mmed took %g\n', toc(startT));
    
    [rocArea_mmed, xamoc_mmed, yamoc_mmed] = ...
        M_SegSVM.test(tst.Ds, tst.segLbs, featOpts.func, detOpts, w_mmed, b_mmed, amocOpt);
    fireFrames_mmed = M_SegSVM.fireFrame_at_FPR(tst.Ds, tst.segLbs, featOpts.func, detOpts, w_mmed, b_mmed, amocOpt, targetFPR);    
    
    fprintf('rocArea_mmed: %g\n', rocArea_mmed);
    plot(xamoc_mmed, yamoc_mmed, 'r', 'LineWidth', lnWdth);
    cnt = cnt+1; legends{cnt} = sprintf('MMED, ROCarea: %.2f', 100*rocArea_mmed);
    
    legend(legends{:}); axis([0, 1, 0, 1]); 

    fprintf('AUC, frm: %g, allfrm: %g, SO: %g, MMED: %g\n', ...
        rocArea_frm, rocArea_allfrm, rocArea_so, rocArea_mmed);
    
    % % Display frames where so and MMED methods fire.
    % Get the indexes of test TS that both so and MMED fire, and there is a target event
    idxs = and(fireFrames_so > 0, fireFrames_mmed > 0);
    idxs = and(idxs, tst.segLbs(2,:) > 0); % there is an event occurs
    fireFrames_so = fireFrames_so(idxs);
    fireFrames_mmed = fireFrames_mmed(idxs);
    tstImDirs = tstImDirs(idxs);
    tstLbs = tstLbs(idxs);
    tstTsLens = tst.segLbs(2,idxs);
    
    idx = 1;
    files = ml_getFilesInDir(tstImDirs{idx}, 'jpg');
    ims{1} = imread(files{1}); % first frame
    ims{2} = imread(files{fireFrames_mmed(idx)}); % frame at which MMED fires
    ims{3} = imread(files{fireFrames_so(idx)}); % frame at which SOSVM fires
    ims{4} = imread(files{end}); % last frame
    bigIm = cat(2, ims{:});
     figure;imshow(bigIm);
     %%setappdata(gcf,'bigIm',bigIm); %imshow(bigIm);
    emotions = {'anger', 'contempt', 'disgust', 'fear', 'happy', 'sad', 'surprise'};
    fprintf('%s\n', tstImDirs{idx});
    fprintf('ims-1-%d-%d-%d_%s\n', fireFrames_mmed(idx), fireFrames_so(idx), tstTsLens(idx), ...
        emotions{tstLbs(idx)});
    
    fntSz = 16;
    xlabel('False Positive Rate', 'FontSize', fntSz); 
    ylabel('Normalized Time to Detect', 'FontSize', fntSz);
    title('AMOC curves and ROC areas for a single run');
    set(gca, 'FontSize', fntSz);
    drawnow;
        
    clear tr tst Ds;
    save(saveFile);
    
% Aggreate results of different runs and plot results    汇总不同运行的结果并绘制结果 
function aggregateRslt(nRun, saveFilePrefix)
    yamoc_frm = cell(1, nRun);
    yamoc_allfrm = yamoc_frm;
    yamoc_so = yamoc_frm;
    yamoc_mmed = yamoc_frm;
        
    rocArea_frm = zeros(1, nRun);
    rocArea_allfrm = rocArea_frm;
    rocArea_so = rocArea_frm;
    rocArea_mmed = rocArea_frm;
    
    for runID=1:nRun
        rslt = load(sprintf('%s_%02d.mat', saveFilePrefix, runID));

        yamoc_frm{runID} = rslt.yamoc_frm;
        yamoc_allfrm{runID} = rslt.yamoc_allfrm;
        yamoc_so{runID} = rslt.yamoc_so;
        yamoc_mmed{runID} = rslt.yamoc_mmed;
                
        rocArea_frm(runID) = rslt.rocArea_frm;
        rocArea_allfrm(runID) = rslt.rocArea_allfrm;
        rocArea_so(runID) = rslt.rocArea_so;
        rocArea_mmed(runID) = rslt.rocArea_mmed;        
        
        xamoc = rslt.xamoc_mmed;
    end;

    yamoc_frm = mean(cat(2,yamoc_frm{:}), 2);
    yamoc_allfrm = mean(cat(2,yamoc_allfrm{:}), 2);
    yamoc_so = mean(cat(2,yamoc_so{:}), 2);
    yamoc_mmed = mean(cat(2,yamoc_mmed{:}), 2);
        
    rocArea_frm_std = std(rocArea_frm);
    rocArea_allfrm_std = std(rocArea_allfrm);
    rocArea_so_std = std(rocArea_so);
    rocArea_mmed_std = std(rocArea_mmed);
    
    rocArea_frm = mean(rocArea_frm);
    rocArea_allfrm = mean(rocArea_allfrm);
    rocArea_so = mean(rocArea_so);
    rocArea_mmed = mean(rocArea_mmed);

    lnWdth = 3; fntSz = 16;    
    figure; clf;    
    plot(xamoc, yamoc_frm, 'm', 'LineWidth',lnWdth, 'LineStyle','-'); hold on;    
    plot(xamoc, yamoc_allfrm, 'g', 'LineWidth',lnWdth, 'LineStyle','-.'); hold on;    
    plot(xamoc, yamoc_so, 'b', 'LineWidth',lnWdth, 'LineStyle','--');
    plot(xamoc, yamoc_mmed, 'r', 'LineWidth',lnWdth, 'LineStyle','-');
    legend( 'frm-peak', 'frm-all', 'SOSVM', 'MMED', 'Location', 'NorthEast');
    xlabel('False Positive Rate', 'FontSize', fntSz); 
    ylabel('Normalized Time to Detect', 'FontSize', fntSz);
    title('AMOC curves combined results from multiple runs');
    set(gca, 'FontSize', fntSz);
    axis square;
    
    fprintf('Aggregation result. rocArea, frm: %g+%g, all_frm: %g+%g, so: %g+%g, MMED: %g+%g\n', ...
        rocArea_frm, rocArea_frm_std, rocArea_allfrm, rocArea_allfrm_std, ...
        rocArea_so, rocArea_so_std, rocArea_mmed, rocArea_mmed_std);

    

% Get the CAPP features, generated by Patrick Lucey
% The CAPP feature is Cannonical Warped Appearance Features
% Outputs:
%   Ds: 1*n cell structure Ds{i} is d*n_i, d: dim, n_i: number of frames for seq i.
%   lbs: 1*n emotion label vector, values from 0 to 7.
%   nFrms: 1*n lengths of sequences
function [Ds, lbs, nFrms, imDirs] = getData()
    dataDir = '../data/CK+/';                      %% dir函数 获得指定文件夹下的所有子文件夹和文件,并存放在在一种为文件结构体数组中. dir函数 可以有调用方式 dir ('.')
    subDirs = ml_getFilesInDir([dataDir, 'Emotion']);   %%subDirs是CK+文件夹里所有的Emotion
    nSubj = length(subDirs);     %%计算向量的长度——Emotion的长度

    featDirs = cell(1, 1000);     
    imDirs = cell(1, 1000);        %%创建1*1000的矩阵
    lbs = zeros(1, 1000);          %%返回一个double的零矩阵
    cnt = 0;
    for i=1:nSubj
        seqDirs = ml_getFilesInDir(subDirs{i});          %%seqDirs是Emotion问价夹里面的
        nSeq = length(seqDirs);
        for j=1:nSeq
            files = ml_getFilesInDir(seqDirs{j}, 'txt');
            if ~isempty(files)
                cnt = cnt + 1;            
                lbs(cnt) = load(files{1});
                featDirs{cnt} = strrep(seqDirs{j}, 'Emotion', 'PatrickLuceyFeats/CAPP/');
                imDirs{cnt} = strrep(seqDirs{j}, 'Emotion', 'cohn-kanade-images_small');
            end;
        end;
    end;
    featDirs = featDirs(1:cnt);
    lbs = lbs(1:cnt);

    imScl = 1/2; % halve the size of the CAPP image, full scale would be too slow.
    mask = imread('../data/CK+/PatrickLuceyFeats/CAPP//S066/002/S066_002_00000001_capp.png');
    mask = imresize(mask, imScl);
    mask = mask > 0;

    Ds = cell(1, cnt);
    d = sum(mask(:));
    nFrms = zeros(1, cnt);
    for i=1:cnt
        ml_progressBar(i, cnt, 'Loading features'); 
        files = ml_getFilesInDir(featDirs{i}, 'png');
        nFrm = length(files);
        nFrms(i) = nFrm;
        Ds{i} = zeros(d, nFrm);
        for j=1:nFrm
            im = imread(files{j});
            im = imresize(im, imScl);
            Ds{i}(:,j) = double(im(mask))/255;
        end;        
    end;
    imDirs = imDirs(1:cnt);

% Given data in Ds, random split and create structure data for training+testing
% Ds: 1*n cell structure
% lbs: 1*n for emotion labels, value from 0 to 7
% nTr: number of training sample per class
% mu_alpha, mu_beta: for MMED
function [tr, tst, tstImDirs, tstLbs] = preData(Ds, lbs, nFrms, imDirs, nTr)
    n = length(Ds);
    % random permutaiton
    idxs = randperm(n);
    Ds = Ds(idxs);
    imDirs = imDirs(idxs);
    lbs = lbs(idxs);
    nFrms = nFrms(idxs);

    % label and # of samples:
    % {0.'neutral': 0, 1. 'anger': 45, 2. 'contempt': 18, 3. 'disgust': 59, 
    %  4.'fear': 25, 5.'happy': 69, 6.'sadness': 28, 7.'surprise': 83};
    
    emotionIdxs = [1, 3, 4, 6]; % negative emotions
    posIdxs = false(size(lbs));
    for i=1:length(emotionIdxs)
        posIdxs = or(posIdxs, lbs == emotionIdxs(i));
    end;
    
    segLbs = zeros(2, n);
    segLbs(1, posIdxs) = 1;
    segLbs(2, posIdxs) = nFrms(posIdxs);

    negIdxs = find(~posIdxs);
    posIdxs = find(posIdxs);

    trIdxs = [posIdxs(1:nTr), negIdxs(1:nTr)];
    tstIdxs = setdiff(1:n, trIdxs);
    tr.Ds = Ds(trIdxs);
    tst.Ds = Ds(tstIdxs);
    tr.segLbs = segLbs(:, trIdxs);
    tst.segLbs = segLbs(:, tstIdxs);
    tstImDirs = imDirs(tstIdxs);
    tstLbs = lbs(tstIdxs);
