CREATE TABLE IF NOT EXISTS mensajes (
    id INT AUTO_INCREMENT PRIMARY KEY,
    mensaje TEXT NOT NULL,
    recibido TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

