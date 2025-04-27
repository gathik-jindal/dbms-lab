insert into Countries values
('C001', 'India', 'IND', 1500),
('C002', 'USA', 'USA', 2000),
('C003', 'France', 'FRA', 1800),
('C004', 'Japan', 'JPN', 1600),
('C005', 'Brazil', 'BRA', 1700);

insert into Airports values
('A001', 'Indira Gandhi Intl', 'C001', 'Delhi, India', 3, 2, 120, 2),
('A002', 'JFK Intl', 'C002', 'New York, USA', 4, 3, 200, 3),
('A003', 'Charles de Gaulle', 'C003',  'Paris, France', 3, 2, 180, 3),
('A004', 'Narita Intl', 'C004', 'Tokyo, Japan', 2, 2, 160, 2),
('A005', 'São Paulo-Guarulhos', 'C005', 'São Paulo, Brazil', 3, 1, 140, 2);


insert into Airlines values
('AL001', 'Air India', 'C001', 100, TRUE),
('AL002', 'Delta Airlines', 'C002', 200, TRUE),
('AL003', 'Air France', 'C003', 150, TRUE),
('AL004', 'ANA', 'C004', 180, TRUE),
('AL005', 'LATAM', 'C005', 120, TRUE);

insert into Slots values
('S001', 'A001', 'AL001', 8.00, 10.00, 30, 60, 'Summer'),
('S002', 'A002', 'AL002', 12.30, 14.00, 20, 45, 'Winter'),
('S003', 'A003', 'AL003', 15.00, 17.00, 25, 50, 'Summer'),
('S004', 'A004', 'AL004', 9.00, 11.00, 30, 60, 'Spring'),
('S005', 'A005', 'AL005', 20.00, 22.00, 35, 70, 'Autumn');

insert into Treaties values
('T001', 'C001', 'C002', 14, '2023-01-01', '2025-12-31'),
('T002', 'C003', 'C004', 10, '2022-06-01', '2024-12-31'),
('T003', 'C001', 'C003', 12, '2023-03-01', '2026-03-01'),
('T004', 'C002', 'C005', 8, '2021-05-01', '2025-05-01'),
('T005', 'C004', 'C005', 9, '2022-10-01', '2026-10-01');

insert into Routes values
('R001', 'A001', 'A002', 12500, 'T001'),
('R002', 'A003', 'A004', 9700, 'T002'),
('R003', 'A001', 'A003', 7000, 'T003'),
('R004', 'A002', 'A005', 8900, 'T004'),
('R005', 'A004', 'A005', 15000, 'T005');

insert into TreatyAirline values
('T001', 'AL001'),
('T001', 'AL002'),
('T002', 'AL003'),
('T003', 'AL001'),
('T004', 'AL005');
