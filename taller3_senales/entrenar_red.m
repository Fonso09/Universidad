% === PASO 1: Cargar imágenes desde carpetas ===
imds = imageDatastore("dataset", ...
    'IncludeSubfolders', true, ...
    'LabelSource', 'foldernames');

% Opcional: balancear clases si tienes pocas
imds = splitEachLabel(imds, min(countEachLabel(imds).Count), 'randomized');

% Tamaño de entrada (puedes ajustar si usas otra resolución)
inputSize = [64 64 3];

% === PASO 2: Redimensionar imágenes ===
augimds = augmentedImageDatastore(inputSize, imds);

% === PASO 3: Definir red CNN sencilla ===
layers = [
    imageInputLayer(inputSize)

    convolution2dLayer(3, 8, 'Padding', 'same')
    batchNormalizationLayer
    reluLayer

    maxPooling2dLayer(2, 'Stride', 2)

    convolution2dLayer(3, 16, 'Padding', 'same')
    batchNormalizationLayer
    reluLayer

    maxPooling2dLayer(2, 'Stride', 2)

    convolution2dLayer(3, 32, 'Padding', 'same')
    batchNormalizationLayer
    reluLayer

    fullyConnectedLayer(numel(categories(imds.Labels)))
    softmaxLayer
    classificationLayer
];

% === PASO 4: Opciones de entrenamiento ===
options = trainingOptions("adam", ...
    'InitialLearnRate', 1e-4, ...
    'MaxEpochs', 10, ...
    'MiniBatchSize', 16, ...
    'Shuffle', 'every-epoch', ...
    'Verbose', false, ...
    'Plots', 'training-progress');

% === PASO 5: Entrenar la red ===
trainedNet = trainNetwork(augimds, layers, options);

% === PASO 6: Guardar la red entrenada ===
save("letrerosNet_simple.mat", "trainedNet");
