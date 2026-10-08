#!/bin/sh
usuario=$(id -un)
hora=$(date +%H)

if [ "$hora" -ge 6 ] && [ "$hora" -lt 12 ]; then
    saludo="Buenos dias"
elif [ "$hora" -ge 12 ] && [ "$hora" -lt 19 ]; then
    saludo="Buenas tardes"
else
    saludo="Buenas noches"
fi

printf '%s, %s. Bienvenido al sistema.\n' "$saludo" "$usuario"
