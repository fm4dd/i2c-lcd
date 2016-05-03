#!/bin/bash
IFS=$'\n'
declare -A ip

for i in $( ifconfig )
do
   if [[ `echo $i | egrep -v '^ '` != "" ]]; then
      ifname=`echo $i | awk '{print $1}'`
      ip[$ifname]=""
   fi
   if [[ `echo $i | grep 'inet addr'` != "" ]]; then
      ip[$ifname]=`echo $i | grep 'inet addr' | awk '{print $2}'| sed 's#addr:##g'`
    fi
done

/srv/scripts/lcdinit

/srv/scripts/lcdwrite 1 "BEAGLEBO"
/srv/scripts/lcdwrite 2 "NE BLACK"
sleep 2
/srv/scripts/lcdwrite 1 "Load:"
/srv/scripts/lcdwrite 2 `uptime |awk '{gsub(/\,/,""); print $10}'`
sleep 2
/srv/scripts/lcdwrite 1 "Memory:"
/srv/scripts/lcdwrite 2 `free -m | grep Mem | awk {'print $3"/"$2 "M"'}`
sleep 2
/srv/scripts/lcdwrite 1 `date +"%m/%d/%y"`
/srv/scripts/lcdwrite 2 `date +%T`

for iface in "${!ip[@]}"
do
   if [ $iface == "lo" ]; then
      continue
   fi
   sleep 4
   /srv/scripts/lcdwrite 1 "$iface IP"
   /srv/scripts/lcdwrite 2 "Address"
   sleep 4
   if [[ `echo ${ip[$iface]}` != "" ]]; then
      /srv/scripts/lcdwrite 1 ${ip[$iface]}
      strlen="${#ip[$iface]}"
      rem=`echo ${ip[$iface]} | cut -c 9-$strlen`
      /srv/scripts/lcdwrite 2 $rem
   else
      /srv/scripts/lcdwrite 1 "= none ="
      /srv/scripts/lcdwrite 2 ""
   fi
done
