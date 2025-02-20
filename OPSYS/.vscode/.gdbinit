set prompt \033[31mgdb$ \033[0m

# kövesse csak az apa folyamatot
# set follow-fork-mode parent

# kövesse csak a fiú folyamatot
# set follow-fork-mode child

# ne váljon le a fiú folyamat
# mindkét folyamatot akarjuk követni
# set detach-on-fork off

# ha vannak argumentumai a programnak
#kommenteljük ezt ki, és adjuk meg őket a feladat szerint
#set args a.txt b.txt