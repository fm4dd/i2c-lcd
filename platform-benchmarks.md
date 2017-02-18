# Rough system and platform performance tests

Date: May 2016

## CPU Performance

```
root@susie:/srv/app/cpuminer/bin# ./minerd --benchmark -a sha256d
root@susie:/srv/app/cpuminer/bin# ./minerd --benchmark
```

System             | CPU              | Arch  | GHz   | OS            |  sha256d (BTC) | scrypt (Litecoin) |
-------------------|------------------|-------|-------|---------------|---------------:|------------------:|
Raspberry Pi 1B    | Broadcom BCM2835 | ARMv6 | 0.7x1 | Raspbian 8    |    171 khash/s |    .28 khash/s    |
Beaglebone Black   | AM3359 Cortex-A8 | ARMv7 | 1.0x1 | Ubuntu 14.04  |    561 khash/s |    .72 khash/s    |
Altera Atlas SoC   | 5CSEMA4U23C6N A9 | ARMv7 | 0.9x2 | Ubuntu 16.04  |  1,016 khash/s |   1.71 khash/s    |
Apple MacMini 2007 | Core 2 Duo T7200 | Intel | 2.0x2 | Windows 10 32 |  5,160 khash/s |   6.15 khash/s    |
Xperia Z4 Tablet   | Snapdragon 810   | ARMv8 | 1.5x4 | Android 6.0   |  5,088 khash/s |   9.70 khash/s    |
HP laptop zBook 14 | i7-4600U Haswell | Intel | 2.7x2 | Windows 10 64 | 20,000 khash/s |  36.00 khash/s    |
Linode 4GB VM      | Xeon E5-2680 VM  | Intel | 2.8x1 | Ubuntu 14.04  | 30,000 khash/s |  48.20 khash/s    |

## I/O Performance

```
root@raspberrypi:/home/pi# sync; dd if=/dev/zero of=~/test.tmp bs=500K count=1024
root@raspberrypi:/home/pi# sync; time dd if=~/test.tmp of=/dev/null bs=500K count=1024
```

System             | OS            | read IO     | write IO     |
-------------------|---------------|------------:|-------------:|
Raspberry PI 1B    | Raspbian 8    |   21.1 MB/s |     7.4 MB/s |
Beaglebone Black   | Ubuntu 14.04  |   34.8 MB/s |    13.1 MB/s |
Altera Atlas SoC   | Ubuntu 16.04  |  431.0 MB/s |    21.7 MB/s |
Xperia Z4 Tablet   | Android 6.0   | 2100.0 MB/s |   110.0 MB/s |
Linode 4GB VM      | Ubuntu 14.04  | 3900.0 MB/s |  1200.0 MB/s |
