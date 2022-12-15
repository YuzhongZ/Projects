function m_main_synData()
% By: Minh Hoai Nguyen (minhhoai@cs.stonybrook.edu)
% Created: 19-Feb-2016
% Last modified: 19-Feb-2016
    
    startT0 = tic;
    nTr  = 100;
    nTst = 100;
    C = 1000;
    [Ds, gtEvs] = genData(nTr, 1);

    muOpts.alpha = 0;
    muOpts.beta  = 1;
    peOpts.lmin = 1;
    peOpts.lstep = 1;
    mvOpts.lmin = 1;
    mvOpts.lmax = inf;
    mvOpts.lstep = 1;
    mvOpts.swStep = 1;
    featOpts.dim = 5;
    featOpts.func = @(x) M_MMED.hist(x, 0:4);
    detOpts = mvOpts;

    [w, b] = M_MMED.train(Ds, gtEvs, C, muOpts, featOpts, peOpts, mvOpts, 'mmed');
    [w_so, b_so] = M_MMED.train(Ds, gtEvs, C, muOpts, featOpts, peOpts, mvOpts, 'sosvm');

    [Ds_tst, gtEvs_tst] = genData(nTst, 0);
    detectScores = cell(1, nTst);
    detectScores_off = cell(1, nTst);
    NTtoDs = zeros(1, nTst);
    NTtoDs_off = zeros(1, nTst);

    nR = 5; nC = 2; lnWdth = 2;
    figure(2); clf;
    for i=1:nTst
        n_i = length(Ds_tst{i});
        lb_i = gtEvs_tst(:,i);

        detectOut = M_MMED.eval(w,b, Ds_tst{i}, featOpts.func, detOpts);
        detectOut_off = M_MMED.eval(w_so,b_so, Ds_tst{i}, featOpts.func, detOpts);

        detectScores{i} = detectOut(3,:);
        detectScores_off{i} = detectOut_off(3,:);
        fstPosIdx = find(detectOut(3,:) > 0, 1);
        fstPosIdx_off = find(detectOut_off(3,:) > 0, 1);
        len_i = lb_i(2) - lb_i(1) + 1;
        NTtoDs(i)     = (fstPosIdx     - lb_i(1)+1)/len_i;
        NTtoDs_off(i) = (fstPosIdx_off - lb_i(1)+1)/len_i;


        % Find the segment with highest score
        [~, highestScoreSegIdx] = max(detectOut(3,:));
        highestScoreSeg = detectOut(1:2,highestScoreSegIdx);

        [~, highestScoreSegIdx] = max(detectOut_off(3,:));
        highestScoreSeg_off = detectOut_off(1:2,highestScoreSegIdx);

        fprintf('Test TS %3d, gt: [%3d %3d], SOSVM: [%3d %3d], MMED: [%3d %3d]\n', ...
            i, gtEvs_tst(1,i), gtEvs_tst(2,i), highestScoreSeg_off(1), highestScoreSeg_off(2), ...
            highestScoreSeg(1), highestScoreSeg(2));

        if (i > nR*nC) % only plot a few time series
            continue;
        end;

        % plot the time series
        subplot(nR, nC, i);
        minY = -1;
        maxY = featOpts.dim ;

        % plot the background part of the time series
        xx = 1:gtEvs_tst(1,i)-1;
        gray = [0.2, 0.2, 0.2];
        plot(xx, Ds_tst{i}(xx), 'color', gray, 'LineWidth', lnWdth); hold on;
        xx = gtEvs_tst(2,i)+1:n_i;
        plot(xx, Ds_tst{i}(xx), 'color', gray, 'LineWidth', lnWdth); hold on;

        % plot the lines marking the moments when detector output positive results.
        line(fstPosIdx*ones(2,1), [minY, maxY], 'color', 'r', 'LineWidth',lnWdth, 'LineStyle','-'); hold on;
        line(fstPosIdx_off*ones(2,1), [minY, maxY], 'color', 'b', 'LineWidth',lnWdth, 'LineStyle','--');

        % plot the event of interest
        xx =gtEvs_tst(1,i)-1:gtEvs_tst(2,i)+1;
        plot(xx, Ds_tst{i}(xx), 'g', 'LineWidth', lnWdth);
        axis([0 200 minY maxY]);
    end;

    % Plot the ROC and AMOC curves
    [xroc, yroc]   = m_getPerfCurve(detectScores, gtEvs_tst,'ROC');
    [xamoc, yamoc] = m_getPerfCurve(detectScores, gtEvs_tst,'AMOC');
    [xroc_off, yroc_off]   = m_getPerfCurve(detectScores_off, gtEvs_tst, 'ROC');
    [xamoc_off, yamoc_off] = m_getPerfCurve(detectScores_off, gtEvs_tst, 'AMOC');

    lnWdth = 3; fntSz = 16;
    figure(3); clf;
    subplot(2,1,1); plot(xroc_off, yroc_off, 'b', 'LineWidth',lnWdth, 'LineStyle','--');
    hold on; plot(xroc, yroc, 'r', 'LineWidth',lnWdth, 'LineStyle','-');
    axis([0 1 0 1]); axis square;
    xlabel('False Positive Rate', 'FontSize', fntSz);
    ylabel('True Postivie Rate', 'FontSize', fntSz);
    legend('SOSVM', 'MMED');
    set(gca, 'FontSize', fntSz);
    title('ROC');

    subplot(2,1,2); plot(xamoc_off, yamoc_off, 'b', 'LineWidth',lnWdth, 'LineStyle','--');
    hold on; plot(xamoc, yamoc, 'r', 'LineWidth',lnWdth,'LineStyle','-');
    axis([0 1 0 1]); axis square;
    xlabel('False Positive Rate', 'FontSize', fntSz);
    ylabel('Normalized Time to Detect', 'FontSize', fntSz);
    legend('SOSVM', 'MMED');
    set(gca, 'FontSize', fntSz);
    title('AMOC');
    fprintf('NTtoD for SOSVM: %g, MMED: %g\n', mean(NTtoDs_off), mean(NTtoDs));
    fprintf('Total running time for m_main_synData: %g\n', toc(startT0));

% Generate synthetic data, the data are simpler (less bins/steps) than genSynData
% D: a cell structure, D{i} is one dimensional vector
% This is the main data for synthetic experiment
function [Ds, gtEvs] = genData(n, shldDisp)
    baseL = 10;
    pat1 = repmat([1 2], baseL, 1); pat1 = pat1(:)';
    pat2 = repmat([-1 -2], baseL, 1); pat2 = pat2(:)';
    pat0 = repmat([1 -1 2 -2], baseL, 1); pat0 = pat0(:)';

    minElem = min(pat0);
    pat1 = pat1 - minElem;
    pat2 = pat2 - minElem;
    pat0 = pat0 - minElem;
    bg = -minElem;

    pats = {pat1, pat2, pat0};
    masks = {zeros(1, length(pat1)), zeros(1, length(pat2)), ones(1, length(pat0))};

    if shldDisp
        % plotting
        figure(1); clf;
        nR = 2; nC = 2; maxX = 50; minY = -1; maxY = 5; bgL = 10;
        fntSz = 12; lnWdth = 2; gray = [0.2, 0.2, 0.2];
        subCnt = 1; subplot(nR, nC, subCnt); hold off;
        plot([bg, pat0, bg], 'r', 'LineWidth', lnWdth);
        axis([0 maxX, minY, maxY]); title('i', 'fontsize', fntSz);

        subCnt = subCnt+1; subplot(nR, nC, subCnt);
        plot([bg*ones(1, bgL), pat1, bg*ones(1, bgL)], 'color', gray, 'LineWidth', lnWdth);
        axis([0 maxX, minY, maxY]); title('ii', 'fontsize', fntSz);

        subCnt = subCnt+1; subplot(nR, nC, subCnt);
        plot([bg*ones(1, bgL), pat2, bg*ones(1, bgL)], 'color', gray, 'LineWidth', lnWdth);
        axis([0 maxX, minY, maxY]); title('iii', 'fontsize', fntSz);

        subCnt = subCnt+1; subplot(nR, nC, subCnt);
        plot(bg*ones(1, 3*bgL), 'color', gray, 'LineWidth', lnWdth);
        axis([0 maxX, minY, maxY]); title('iv', 'fontsize', fntSz);
    end;

    Ds = cell(1, n);
    gtEvs = zeros(2, n);
    for t=1:n
        idxs = randi(2, [1,3]); % which of pat1, pat2 to use?
        pat0_pos = randi(3); % is the pat in the left, center, right of two of {pat1, pat2}?
        idxs(pat0_pos) = 3;
        segs = cell(1, 7);
        segs([2,4,6]) = pats(idxs);
        lb = cell(1, 7);
        lb([2,4,6]) = masks(idxs);
        for i=1:4
            l_i = randi([2*baseL, baseL*3]);
            segs{2*i-1} = bg*ones(1, l_i);
            lb{2*i-1} = zeros(1, l_i);
        end;
        Ds{t} = cat(2, segs{:});
        lb = cat(2, lb{:});
        lb = find(lb);
        gtEvs(:,t) = [lb(1), lb(end)];
    end;
