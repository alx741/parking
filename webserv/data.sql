/* mysql -u alx -p -h localhost < data.sql */

use Parking;

DELETE FROM puesto;
DELETE FROM bloque;
DELETE FROM usuario;
DELETE FROM edificio;

INSERT INTO edificio
VALUES (0, "Ingenieria", 2, 0, -2.9010587, -79.0097391);

INSERT INTO usuario (nombre, email, placa, edificioId)
VALUES
    ("Alysa Stanton","Abdullah@henri.us","281", 1),
    ("Vilma Runolfsson I","Roslyn_Luettgen@deonte.ca","62", 1),
    ("Clyde Parker","Zane.Bayer@rebeka.net","691", 1),
    ("Era DuBuque","Carlie@damaris.tv","196", 1),
    ("Colt Simonis","Dasia@hallie.net","122", 1),
    ("Schuyler Jenkins","Aurelie_Collier@valerie.com","712", 1),
    ("Maryjane Wiza DDS","Greyson@murl.io","580", 1),
    ("Hilda Botsford","Justus@ollie.info","744", 1),
    ("Ardella Yost","Elbert.Harris@treva.ca","525", 1),
    ("Nathaniel Kulas DVM","Valerie@sadye.tv","493", 1);


INSERT INTO bloque
VALUES
    (1, 1, -2.9010587, -79.0097391, 15, 0, 0, NOW()),
    (2, 1, -2.9010587, -79.0097391, 7, 0, 0, NOW());

INSERT INTO puesto (bloqueId, puesto, ocupado, reservado)
VALUES
    (1, 1, 0, 0),
    (1, 2, 0, 0),
    (1, 3, 0, 0),
    (1, 4, 0, 0),
    (1, 5, 0, 0),
    (1, 6, 0, 0),
    (1, 7, 0, 0),
    (1, 8, 0, 0),
    (1, 9, 0, 0),
    (1, 10, 0, 0),
    (1, 11, 0, 0),
    (1, 12, 0, 0),
    (1, 13, 0, 0),
    (1, 14, 0, 0),
    (1, 15, 0, 0),

    (2, 1, 0, 0),
    (2, 2, 0, 0),
    (2, 3, 0, 0),
    (2, 4, 0, 0),
    (2, 5, 0, 0),
    (2, 6, 0, 0),
    (2, 7, 0, 0);
