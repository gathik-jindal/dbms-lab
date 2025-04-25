insert into department(dname, dnumber, mgr_ssn, mgr_start_date) values ('IT Department', 3, 888665555, '1981-06-19');
insert into dept_locations(dnumber, dlocation) values (3, 'Jaynagar');
insert into employee(fname,lname,ssn,sex,super_ssn,dno) VALUES ('Simon', 'Tan', 97868565, 'M', 888665555, 3);
insert into project(pname, pnumber, plocation, dnum) values ('HR Automation', 6, 'Jaynagar', 3);
insert into works_on(essn, pno, hours) values (97868565, 6, 40.0);
insert into dependent(essn, dependent_name, sex, bdate, relationship) values (97868565, 'Gourav Anirudh', 'M', '2005-05-05', 'Father');
insert into dependent(essn, dependent_name, sex, bdate, relationship) values (97868565, 'Gourav Anirudh 2', 'M', '2005-05-05', 'Wife');