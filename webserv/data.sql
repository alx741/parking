use Parking;

DELETE FROM Puesto;
DELETE FROM Bloque;
DELETE FROM Usuario;
DELETE FROM Edificio;

INSERT INTO Edificio
VALUES (1, "Ingeniería", 2, 0, -2.9010587, -79.0097391);

INSERT INTO Usuario (nombre, email, placa, edificioId)
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


INSERT INTO Bloque
VALUES
    (1, 1, -2.9010587, -79.0097391, 0, 0, 0, NOW()),
    (2, 1, -2.9010587, -79.0097391, 0, 0, 0, NOW());

INSERT INTO Puesto (bloqueId, ocupado)
VALUES
    (1, 0),
    (1, 0),
    (1, 0),
    (2, 0),
    (2, 0),
    (2, 0);
