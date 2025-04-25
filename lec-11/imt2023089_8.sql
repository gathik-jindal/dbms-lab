select e.fname, e.minit, e.lname from employee as e where not exists
(
select * 
from works_on as w 
where e.ssn = w.essn
);