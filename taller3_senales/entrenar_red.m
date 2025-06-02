% % === PASO 1: Cargar imágenes desde carpetas ===
% imds = imageDatastore("dataset", ...
%     'IncludeSubfolders', true, ...
%     'LabelSource', 'foldernames');
% 
% % Opcional: balancear clases si tienes pocas
% imds = splitEachLabel(imds, min(countEachLabel(imds).Count), 'randomized');
% 
% 
% inputSize = [64 64 3];
% 
% % aumentar dataset
% augimds = augmentedImageDatastore(inputSize, imds);
% 
% layers = [
%     imageInputLayer(inputSize)
% 
%     convolution2dLayer(3, 8, 'Padding', 'same')
%     batchNormalizationLayer
%     reluLayer
% 
%     maxPooling2dLayer(2, 'Stride', 2)
% 
%     convolution2dLayer(3, 16, 'Padding', 'same')
%     batchNormalizationLayer
%     reluLayer
% 
%     maxPooling2dLayer(2, 'Stride', 2)
% 
%     convolution2dLayer(3, 32, 'Padding', 'same')
%     batchNormalizationLayer
%     reluLayer
% 
%     fullyConnectedLayer(numel(categories(imds.Labels)))
%     softmaxLayer
%     classificationLayer
% ];
% 
% options = trainingOptions("adam", ...
%     'InitialLearnRate', 1e-4, ...
%     'MaxEpochs', 10, ...
%     'MiniBatchSize', 16, ...
%     'Shuffle', 'every-epoch', ...
%     'Verbose', false, ...
%     'Plots', 'training-progress');
% 
% %entrenar
% trainedNet = trainNetwork(augimds, layers, options);
% save("letrerosNet_simple.mat", "trainedNet");

%%

x = [ 118 105 89 80 78 82 82 84 82 75 72 63 56 52 51 46 47 49 44 41 40 46 45 47 44 45 49 50 50 49 48 51 50 51 49 47 46 50 45 54 52 51 40 40 42 46 44 46 43 46 47 44 42 41 46 42 38 42 40 41 44 51 53 51 38 36 41 43 35 38 44 49 51 47 48 50 53 45 50 46 50 47 46 48 49 52 47 49 44 42 39 48 40 43 44 41 41 42 44 46 43 44 41 40 42 42 42 42 43 45 42 44 43 40 42 42 40 42 42 42 42 41 44 43 44 38 41 44 42 41 42 43 42 40 44 42 40 41 44]

y = ones(1, 139) * 45;