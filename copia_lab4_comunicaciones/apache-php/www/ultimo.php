<?php
$host = getenv("MYSQL_HOST");
$user = getenv("MYSQL_USER");
$pass = getenv("MYSQL_PASSWORD");
$db   = getenv("MYSQL_DATABASE");

$conn = new mysqli($host, $user, $pass, $db);
if ($conn->connect_error) {
    die("Error de conexión: " . $conn->connect_error);
}

$sql = "SELECT * FROM mensajes ORDER BY recibido DESC LIMIT 1";
$result = $conn->query($sql);

if ($result && $row = $result->fetch_assoc()) {
    echo "<p><strong>Mensaje:</strong> " . htmlspecialchars($row['mensaje']) . "</p>";
    echo "<p><strong>Recibido:</strong> " . $row['recibido'] . "</p>";
} else {
    echo "<p>No hay mensajes todavía.</p>";
}

$conn->close();
?>

