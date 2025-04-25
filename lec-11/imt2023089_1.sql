insert into employee(fname,minit,lname,ssn,bdate,address,sex,salary,super_ssn,dno) VALUES ('Jane', NULL, 'Smith', 123456, NULL, NULL, 'F', NULL, 888665555, 5);
insert into dept_locations(dnumber, dlocation) values (5, 'MG Road');
insert into project(pname, pnumber, plocation, dnum) values ('NextGen Computing', 4, 'MG Road', 5);
insert into works_on(essn, pno, hours) values (123456, 4, 20.0);
insert into dependent(essn, dependent_name, sex, bdate, relationship) values (123456, 'Gourav Anirudh', 'M', '2005-05-05', 'Husband');