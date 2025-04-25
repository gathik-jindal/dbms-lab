select e.fname, e.minit, e.lname 
from employee as e, works_on as w 
where e.ssn = w.essn 
and w.hours = (select max(hours) from works_on);