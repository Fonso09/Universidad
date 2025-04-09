<!DOCTYPE html>
<html>
<head>
    <title>Último mensaje de ESP32</title>
    <meta charset="UTF-8">
    <style>
 body {
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        background: linear-gradient(135deg, #e9f0ff, #ffffff);
        margin: 0;
        padding: 2rem;
        display: flex;
        flex-direction: column;
        align-items: center;
        color: #333;
    }

    h1 {
        color: #2c3e50;
        margin-bottom: 1.5rem;
        text-align: center;
    }

    #mensaje {
        background-color: #ffffff;
        border-radius: 12px;
        box-shadow: 0 8px 20px rgba(0, 0, 0, 0.08);
        padding: 1.5rem 2rem;
        max-width: 500px;
        width: 100%;
        transition: all 0.3s ease;
    }

    #mensaje p {
        margin: 0.5rem 0;
        font-size: 1.1rem;
    }

    #mensaje p strong {
        color: #1f4f99;
    }

    @media (max-width: 600px) {
        body {
            padding: 1rem;
        }

        #mensaje {
            padding: 1rem;
        }
    }
    </style>
    <script>
        function actualizarMensaje() {
            fetch("ultimo.php")
                .then(response => response.text())
                .then(data => {
                    document.getElementById("mensaje").innerHTML = data;
                });
        }
        setInterval(actualizarMensaje, 1000); // Actualiza cada 1 segundos
        window.onload = actualizarMensaje;
    </script>
</head>
<body>
    <h1>Último mensaje recibido de la ESP32</h1>
    <div id="mensaje">Cargando mensaje...</div>
</body>
</html>

