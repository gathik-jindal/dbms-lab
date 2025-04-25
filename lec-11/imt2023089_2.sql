insert into department(dname, dnumber, mgr_ssn, mgr_start_date) values ('Finanace', 2, 888665555, '1981-06-19');
insert into dept_locations(dnumber, dlocation) values (2, 'Electronic City');
insert into employee(fname,lname,ssn,sex,super_ssn,dno) VALUES ('Peter', 'Pan', 3343443, 'M', 888665555, 2);
insert into project(pname, pnumber, plocation, dnum) values ('Smart Card Implementation', 5, 'Electronic City', 2);
insert into works_on(essn, pno, hours) values (3343443, 5, 30.0);