select pname
from project,1 employee, works_on
where employee.fname='Joyce'
and employee.lname='English'
and employee.ssn=works_on.essn
and works_on.pno=project.pnumber;