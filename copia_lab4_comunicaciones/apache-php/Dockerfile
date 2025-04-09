FROM php:8.2-apache

# Instalar extensión mysqli
RUN docker-php-ext-install mysqli && docker-php-ext-enable mysqli

# Copiar archivos PHP al contenedor
COPY ./www /var/www/html

# Asegurarse de que Apache tenga permisos sobre los archivos
RUN chown -R www-data:www-data /var/www/html
