<?php
$host = getenv("MYSQL_HOST");
$user = getenv("MYSQL_USER");
$pass = getenv("MYSQL_PASSWORD");
$db   = getenv("MYSQL_DATABASE");

$conn = new mysqli($host, $user, $pass, $db);

if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $mensaje = file_get_contents("php://input");
    $stmt = $conn->prepare("INSERT INTO mensajes (mensaje) VALUES (?)");
    $stmt->bind_param("s", $mensaje);
    $stmt->execute();
    $stmt->close();
    echo "OK";
} else {
    echo "Usá POST, no GET.";
}

$conn->close();
?>

