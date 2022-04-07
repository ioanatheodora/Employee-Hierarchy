     Ierarhie

    Cerinta 1:
1) hire: se creeaza un nod in care se aloca memorie pentru numele angajatului 
si vectorul de angajati subordonati lui; in functie de numele manager-ului( 
daca este NULL sau un sir de caractere), se returneaza nodul creat deja, 
respectiv nodul este atasat in echipa manager-ului din care va face parte
si se returneaza arborele initial.

2) fire: gasim in arbore nodul asociat angajatul cu numele employee_name, 
verificam conditiile de a exista acest angajat, respectiv sa nu fie primul
angajat din ierarhie, eliminam angajatul din echipa manager-ului sau si
updatam numarul de angajati directi, adaugam oamenii subordonati celui concediat
in echipa manager-ului si eliberam memoria pentru cel concediat.

3) promote: gasim nodul asociat angajatului cu numele employee_name si verificam
ca acesta sa se afle de la nivelul 2 in jos; eliminam angajatul din echipa
manager-ului, adaugam angajatii subordonati celui promovat in echipa manager-ului
si se adauga angajatul promovat in echipa manager-ului cu un nivel mai sus.

4) move_employee: se procedeaza analog ca la promote, insa angajatul va fi
adaugat in echipa manager-ului "new_manager"

5) move_team: procedam analog ca la move_employee, fara a mai muta angajatii
subordonati in echipa manager-ului vechi

6) fire_team: gasim nodul angajatului a carui echipa va fi concediata, eliminam
angajatul din echipa manager-ului sau si eliberam memoria pentru arborele
asociat echipei angajate.


    Cerinta 2:
1) get_employees_by_manager: gasim nodul asociat angajatului "employee_name",
parcurgem arborele avand ca radacina nodul gasit si retinem numele angajatilor
in matricea employees, pe care o vom ordona alfabetic.

2) get_employees_by_level: parcurgem arborele, calculand  nivelul pentru fiecare
nod la care se ajunge. Daca este nivelul nodului este egal cu cel dat drept 
parametru, se adauga in matricea in care retinem numele angajatilor. Dupa ce se
parcurge, ordonam angajatii alfabetic si se afiseaza.

3) get_best_manager: gasim numarul maxim de angajati directi pe care ii are
un manager, parcurgem arborele si retinem managerii cu acest numar de angajati
directi in matricea employees. Ordonam angajatii alfabetic si ii afisam.

    
    Cerinta 3:
    Gasim nodul asociat angajatului dorit(head); plecand de la manager-ul
angajatului gasit pana la radacina initiala a arborelui, manager-ul angajatului
de la pasul curent este adaugat in echipa angajatului.
    Se returneaza nodul head. 