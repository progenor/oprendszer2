#!/bin/bash
#példák letöltése a tananyaghoz
#használat (amennyiben a megadott lab2_file.tgz, stb. letölthető):
#  ./ushell.sh lab2_file.tgz
#  ./ushell.sh lab2_file.tgz lab3_fork.tgz

# !!! felülírja az előző fájlokat értesítés nélkül
# ha a letöltött könyvtárakban saját könyvtárat hozunk létre
# azt nem módosítja második letöltéskor

#hibakiíró függvény a standard hibakimenetre
#a hiba kiírása után kilép a szkript
#$0 a szkript neve
errExit() {
    echo "$0: $*" >&2
    exit 1
}

#web könyvtár ahonnan letöltünk
URL="https://www.ms.sapientia.ro/~lszabo/USHELL"

#ha nincs egy paraméter sem
if [[ -z "$1" ]]; then
    errExit "használat: $0 fájl1 [fájl2 ...]"
fi

#minden parancssoron megadott fájlra
for file; do
    #letöltés
    if ! wget -nv -O "$file" "$URL/$file"; then
        #a -O létrehozza a fájlt akkor is ha sikertelen a letöltés, töröljük
        if [[ -f "$file" ]]; then
            rm "$file"
        fi
        errExit "wget hiba: $file letöltése nem sikerült"
    fi

    #ha tgz vagy zip:akkor kipakolás, ha egyéb semmi további művelet
    case "$file" in
    *.tgz)
        if ! tar -xzf "$file"; then
            errExit "tar hiba: $file-t nem sikerült kipakolni"
        fi
        ;;
    *.zip)
        if ! unzip -q -o "$file"; then
            errExit "unzip hiba: $file-t nem sikerült kipakolni"
        fi
        ;;
    *) ;; #egyelőre nincs művelet
    esac

    #kis szünet két web kérés között
    sleep 0.5
done
