function clasificador_gui
    % Cargar red entrenada
    data = load("letrerosNet_simple.mat");
    net = data.trainedNet;

    f = figure('Name', 'Clasificador de Letreros', ...
               'Position', [500 300 600 400]);

    uicontrol('Style', 'pushbutton', ...
              'String', 'Cargar Imagen', ...
              'FontSize', 12, ...
              'Position', [50 20 150 40], ...
              'Callback', @cargarImagen);

    ax = axes('Parent', f, ...
              'Units', 'pixels', ...
              'Position', [200 80 350 300]);

    
    resultadoText = uicontrol('Style', 'text', ...
                              'String', 'Resultado: ', ...
                              'FontSize', 14, ...
                              'HorizontalAlignment', 'left', ...
                              'Position', [220 30 300 30]);

  
    function cargarImagen(~, ~)
        [file, path] = uigetfile({'*.jpg;*.png;*.jpeg', 'Imágenes'});
        if isequal(file, 0)
            return;
        end

        img = imread(fullfile(path, file));
        imshow(img, 'Parent', ax);
        imgResize = imresize(img, net.Layers(1).InputSize(1:2));
        label = classify(net, imgResize);
        resultadoText.String = ['Resultado: ', char(label)];
    end
end
