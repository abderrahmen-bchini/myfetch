#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <sys/statvfs.h>
#include <dirent.h>


static const char *default_ascii[] = {
    "███╗   ███╗██╗   ██╗    ███████╗███████╗████████╗ ██████╗██╗  ██╗",
    "████╗ ████║╚██╗ ██╔╝    ██╔════╝██╔════╝╚══██╔══╝██╔════╝██║  ██║",
    "██╔████╔██║ ╚████╔╝     █████╗  █████╗     ██║   ██║     ███████║",
    "██║╚██╔╝██║  ╚██╔╝      ██╔══╝  ██╔══╝     ██║   ██║     ██╔══██║",
    "██║ ╚═╝ ██║   ██║       ██║     ███████╗   ██║   ╚██████╗██║  ██║",
    "╚═╝     ╚═╝   ╚═╝       ╚═╝     ╚══════╝   ╚═╝    ╚═════╝╚═╝  ╚═╝"
};
static const int default_ascii_lines = 6;

static const char *arch_logo[] = {
    "\033[38;5;39m                   -`\033[0m",
    "\033[38;5;39m                  .o+`\033[0m",
    "\033[38;5;39m                 `ooo/\033[0m",
    "\033[38;5;39m                `+oooo:\033[0m",
    "\033[38;5;39m               `+oooooo:\033[0m",
    "\033[38;5;39m               -+oooooo+:\033[0m",
    "\033[38;5;39m             `/:-:++oooo+:\033[0m",
    "\033[38;5;39m            `/++++/+++++++:\033[0m",
    "\033[38;5;39m           `/++++++++++++++:\033[0m",
    "\033[38;5;39m          `/+++o\033[38;5;254moooooooo\033[38;5;39moooo/`\033[0m",
    "\033[38;5;254m         \033[38;5;39m./\033[38;5;254mooosssso++osssssso\033[38;5;39m+`\033[0m",
    "\033[38;5;254m        .oossssso-````/ossssss+`\033[0m",
    "\033[38;5;254m       -osssssso.      :ssssssso.\033[0m",
    "\033[38;5;254m      :osssssss/        osssso+++.\033[0m",
    "\033[38;5;254m     /ossssssss/        +ssssooo/-\033[0m",
    "\033[38;5;254m   `/ossssso+/:-        -:/+osssso+-\033[0m",
    "\033[38;5;254m  `+sso+:-`                 `.-/+oso:\033[0m",
    "\033[38;5;254m `++:.                           `-/+/\033[0m",
    "\033[38;5;254m .`                                 `/\033[0m"
};
static const int arch_logo_lines = 19;

static const char *debian_logo[] = {
    "\033[38;5;251m       _,met$$$$$gg.\033[0m",
    "\033[38;5;251m    ,g$$$$$$$$$$$$$$$P.\033[0m",
    "\033[38;5;251m  ,g$$P\\\"        \\\"\\\"\\\"Y$$\\\".\".\033[0m",
    "\033[38;5;251m ,$$P'              `$$$.\033[0m",
    "\033[38;5;251m',$$P       ,ggs.     `$$b:\033[0m",
    "\033[38;5;251m`d$$'     ,$P\\\"'   \033[38;5;161m.\033[38;5;251m    $$$\033[0m",
    "\033[38;5;251m $$P      d$'     \033[38;5;161m,\033[38;5;251m    $$P\033[0m",
    "\033[38;5;251m $$:      $$.   \033[38;5;161m-\033[38;5;251m    ,d$$'\033[0m",
    "\033[38;5;251m $$;      Y$b._   _,d$P'\033[0m",
    "\033[38;5;251m Y$$.    \033[38;5;161m`.\033[38;5;251m`\\\"Y$$$$P\\\"'\033[0m",
    "\033[38;5;251m `$$b      \033[38;5;161m\\\"-.__\033[0m",
    "\033[38;5;251m  `Y$$\033[0m",
    "\033[38;5;251m    `Y$$.\033[0m",
    "\033[38;5;251m      `$$b.\033[0m",
    "\033[38;5;251m        `Y$$b.\033[0m",
    "\033[38;5;251m           `\\\"Y$b._\033[0m",
    "\033[38;5;251m               `\\\"\\\"\\\"\033[0m"
};
static const int debian_logo_lines = 17;

static const char *ubuntu_logo[] = {
    "\033[38;5;202m            .-\\/+oossssoo+\\-.            \033[0m",
    "\033[38;5;202m         ´:+ssssssssssssssssss+:`         \033[0m",
    "\033[38;5;202m       -+ssssssssssssssssssyyssss+-       \033[0m",
    "\033[38;5;202m     .ossssssssssssssssss\033[38;5;254mdMMMNy\033[38;5;202msssso.     \033[0m",
    "\033[38;5;202m    /sssssssssss\033[38;5;254mhdmmNNmmyNMMMMh\033[38;5;202mssssss\\    \033[0m",
    "\033[38;5;202m   +sssssssss\033[38;5;254mhm\033[38;5;202myd\033[38;5;254mMMMMMMMNddddy\033[38;5;202mssssssss+   \033[0m",
    "\033[38;5;202m  /ssssssss\033[38;5;254mhNMMM\033[38;5;202myh\033[38;5;254mhyyyyhmNMMMNh\033[38;5;202mssssssss\\  \033[0m",
    "\033[38;5;202m .ssssssss\033[38;5;254mdMMMNh\033[38;5;202mssssssssss\033[38;5;254mhNMMMd\033[38;5;202mssssssss. \033[0m",
    "\033[38;5;202m +ssss\033[38;5;254mhhhyNMMNy\033[38;5;202mssssssssssss\033[38;5;254myNMMMy\033[38;5;202msssssss+ \033[0m",
    "\033[38;5;202m oss\033[38;5;254myNMMMNyMMh\033[38;5;202mssssssssssssss\033[38;5;254mhmmmh\033[38;5;202mssssssso \033[0m",
    "\033[38;5;202m oss\033[38;5;254myNMMMNyMMh\033[38;5;202msssssssssssssshmmmh\033[38;5;202mssssssso \033[0m",
    "\033[38;5;202m +ssss\033[38;5;254mhhhyNMMNy\033[38;5;202mssssssssssss\033[38;5;254myNMMMy\033[38;5;202msssssss+ \033[0m",
    "\033[38;5;202m .ssssssss\033[38;5;254mdMMMNh\033[38;5;202mssssssssss\033[38;5;254mhNMMMd\033[38;5;202mssssssss. \033[0m",
    "\033[38;5;202m  \\ssssssss\033[38;5;254mhNMMM\033[38;5;202myh\033[38;5;254mhyyyyhdNMMMNh\033[38;5;202mssssssss/  \033[0m",
    "\033[38;5;202m   +sssssssss\033[38;5;254mdm\033[38;5;202myd\033[38;5;254mMMMMMMMMddddy\033[38;5;202mssssssss+   \033[0m",
    "\033[38;5;202m    \\sssssssssss\033[38;5;254mhdmNNNNmyNMMMMh\033[38;5;202mssssss/    \033[0m",
    "\033[38;5;202m     .ossssssssssssssssss\033[38;5;254mdMMMNy\033[38;5;202msssso.     \033[0m",
    "\033[38;5;202m       -+sssssssssssssssss\033[38;5;254myyy\033[38;5;202mssss+-       \033[0m",
    "\033[38;5;202m         `:+ssssssssssssssssss+:`         \033[0m",
    "\033[38;5;202m             .-\\+oossssoo+/-.             \033[0m"
};
static const int ubuntu_logo_lines = 20;

static const char *fedora_logo[] = {
    "\033[38;5;27m             .',;::::;,'.\033[0m",
    "\033[38;5;27m          .';:cccccccccccc:;,.\033[0m",
    "\033[38;5;27m       .;cccccccccccccccccccccc;.\033[0m",
    "\033[38;5;27m     .:cccccccccccccccccccccccccc:.\033[0m",
    "\033[38;5;27m   .;ccccccccccccc;\033[38;5;254m.:dddl:.\033[38;5;27m;ccccccc;.\033[0m",
    "\033[38;5;27m  .:ccccccccccccc;\033[38;5;254mOWMKOOXMWd\033[38;5;27m;ccccccc:.\033[0m",
    "\033[38;5;27m .:ccccccccccccc;\033[38;5;254mKMMc\033[38;5;27m;cc;\033[38;5;254mxMMc\033[38;5;27m;ccccccc:.\033[0m",
    "\033[38;5;27m ,cccccccccccccc;\033[38;5;254mMMM.\033[38;5;27m;cc;\033[38;5;254m;WW:\033[38;5;27m;cccccccc,\033[0m",
    "\033[38;5;27m :cccccccccccccc;\033[38;5;254mMMM.\033[38;5;27m;cccccccccccccccc:\033[0m",
    "\033[38;5;27m :ccccccc;\033[38;5;254moxOOOo\033[38;5;27m;\033[38;5;254mMMM0OOk.\033[38;5;27m;cccccccccccc:\033[0m",
    "\033[38;5;27m cccccc;\033[38;5;254m0MMKxdd:\033[38;5;27m;\033[38;5;254mMMMkddc.\033[38;5;27m;cccccccccccc;\033[0m",
    "\033[38;5;27m ccccc;\033[38;5;254mXM0'\033[38;5;27m;cccc;\033[38;5;254mMMM.\033[38;5;27m;cccccccccccccccc'\033[0m",
    "\033[38;5;27m ccccc;\033[38;5;254mMMo\033[38;5;27m;ccccc;\033[38;5;254mMMW.\033[38;5;27m;ccccccccccccccc;\033[0m",
    "\033[38;5;27m ccccc;\033[38;5;254m0MNc.\033[38;5;27mccc\033[38;5;254m.xMMd\033[38;5;27m;ccccccccccccccc;\033[0m",
    "\033[38;5;27m cccccc;\033[38;5;254mdNMWXXXWM0:\033[38;5;27m;cccccccccccccc:,\033[0m",
    "\033[38;5;27m cccccccc;\033[38;5;254m.:odl:.\033[38;5;27m;cccccccccccccc:,.\033[0m",
    "\033[38;5;27m :cccccccccccccccccccccccccccc:'.\033[0m",
    "\033[38;5;27m .:cccccccccccccccccccccc:;,..\033[0m",
    "\033[38;5;27m   '::cccccccccccccc::;,.\033[0m"
};
static const int fedora_logo_lines = 19;

static const char *alpine_logo[] = {
    "\033[38;5;32m       .hddddddddddddddddddddddh.\033[0m",
    "\033[38;5;32m       :dddddddddddddddddddddddddd:\033[0m",
    "\033[38;5;32m      /dddddddddddddddddddddddddddd/\033[0m",
    "\033[38;5;32m     +dddddddddddddddddddddddddddddd+\033[0m",
    "\033[38;5;32m   `sdddddddddddddddddddddddddddddddds`\033[0m",
    "\033[38;5;32m  `ydddddddddddd++hdddddddddddddddddddy`\033[0m",
    "\033[38;5;32m .hddddddddddd+`  `+ddddh:-sdddddddddddh.\033[0m",
    "\033[38;5;32m hdddddddddd+`      `+y:    .sddddddddddh\033[0m",
    "\033[38;5;32m ddddddddh+`   `//`   `.`     -sddddddddd\033[0m",
    "\033[38;5;32m ddddddh+`   `/hddh/`   `:s-    -sddddddd\033[0m",
    "\033[38;5;32m ddddh+`   `/+/dddddh/`   `+s-    -sddddd\033[0m",
    "\033[38;5;32m ddd+`   `/o` :dddddddh/`   `oy-    .yddd\033[0m",
    "\033[38;5;32m hdddyo+ohddyosdddddddddho+oydddy++ohdddh\033[0m",
    "\033[38;5;32m .hddddddddddddddddddddddddddddddddddddh.\033[0m",
    "\033[38;5;32m  `yddddddddddddddddddddddddddddddddddy`\033[0m",
    "\033[38;5;32m   `sdddddddddddddddddddddddddddddddds`\033[0m",
    "\033[38;5;32m     +dddddddddddddddddddddddddddddd+\033[0m",
    "\033[38;5;32m      /dddddddddddddddddddddddddddd/\033[0m"
};
static const int alpine_logo_lines = 18;

static const char *kali_logo[] = {
    "\033[38;5;39m..............\033[0m",
    "\033[38;5;39m            ..,;:ccc,.\033[0m",
    "\033[38;5;39m          ......''';lxO.\033[0m",
    "\033[38;5;39m.....''''..........,:ld;\033[0m",
    "\033[38;5;39m            .';;;:::;,,.x,\033[0m",
    "\033[38;5;39m       ..'''.            0Xxoc:,.  ...\033[0m",
    "\033[38;5;39m   ....                ,ONkc;,;cokOdc',\033[0m",
    "\033[38;5;39m  .                   OMo           ':\033[38;5;254mdd\033[38;5;39mo.\033[0m",
    "\033[38;5;39m                     dMc               :OO;\033[0m",
    "\033[38;5;39m                     0M.                 .:o.\033[0m",
    "\033[38;5;39m                     ;Wd\033[0m",
    "\033[38;5;39m                      ;XO,\033[0m",
    "\033[38;5;39m                        ,d0Odlc;,..\033[0m",
    "\033[38;5;39m                            ..',;:cdOOd::,.\033[0m",
    "\033[38;5;39m                                     .:d;.\033[38;5;254m':;.\033[0m",
    "\033[38;5;39m                                        'd,  .'\033[0m",
    "\033[38;5;39m                                          ;l   ..\033[0m",
    "\033[38;5;39m                                           .o\033[0m",
    "\033[38;5;39m                                             c\033[0m",
    "\033[38;5;39m                                             .'\033[0m",
    "\033[38;5;39m                                              .\033[0m"
};
static const int kali_logo_lines = 21;

static const char *gentoo_logo[] = {
    "\033[38;5;99m         -/oyddmdhs+:.\033[0m",
    "\033[38;5;99m      -o\033[38;5;254mdNMMMMMMMMNNmhy+\033[38;5;99m-`\033[0m",
    "\033[38;5;99m    -y\033[38;5;254mNMMMMMMMMMMMNNNmmdhy\033[38;5;99m+-\033[0m",
    "\033[38;5;99m  `o\033[38;5;254mmMMMMMMMMMMMMNmdmmmmddhhy\033[38;5;99m/`\033[0m",
    "\033[38;5;99m  om\033[38;5;254mMMMMMMMMMMMN\033[38;5;99mhhyyyo\033[38;5;254mhmdddhhhd\033[38;5;99mo`\033[0m",
    "\033[38;5;99m .y\033[38;5;254mdMMMMMMMMMMd\033[38;5;99mhs++so/s\033[38;5;254mdmdddhhhhdm\033[38;5;99m+`\033[0m",
    "\033[38;5;99m  oy\033[38;5;254mhdmNMMMMMMMN\033[38;5;99mdyooy\033[38;5;254mdmddddhhhhyhN\033[38;5;99md.\033[0m",
    "\033[38;5;99m   :o\033[38;5;254myhhdNNMMMMMMMNNNmmdddhhhhhyym\033[38;5;99mMh\033[0m",
    "\033[38;5;99m     .:\033[38;5;254m+sydNMMMMMNNNmmmdddhhhhhhmM\033[38;5;99mmy\033[0m",
    "\033[38;5;99m        /m\033[38;5;254mMMMMMMNNNmmmdddhhhhhmMNh\033[38;5;99ms:\033[0m",
    "\033[38;5;99m     `o\033[38;5;254mNMMMMMMMNNNmmmddddhhdmMNhs\033[38;5;99m+`\033[0m",
    "\033[38;5;99m   `s\033[38;5;254mNMMMMMMMMNNNmmmdddddmNMmhs\033[38;5;99m/.\033[0m",
    "\033[38;5;99m  /N\033[38;5;254mMMMMMMMMNNNNmmmdddmNMNdso\033[38;5;99m:`\033[0m",
    "\033[38;5;99m +M\033[38;5;254mMMMMMMNNNNNmmmmdmNMNdso\033[38;5;99m/-\033[0m",
    "\033[38;5;99m yM\033[38;5;254mMNNNNNNmmmmmNNMmhs+/\033[38;5;99m-`\033[0m",
    "\033[38;5;99m /h\033[38;5;254mMMNNNNNNNNMNdhs++/\033[38;5;99m-`\033[0m",
    "\033[38;5;99m `/\033[38;5;254mohdmmddhys+++/:\033[38;5;99m.`\033[0m",
    "\033[38;5;99m   `-//////:--.\033[0m"
};
static const int gentoo_logo_lines = 18;

static const char *mint_logo[] = {
    "\033[38;5;10m             ...-:::::-...\033[0m",
    "\033[38;5;10m          .-MMMMMMMMMMMMMMM-.\033[0m",
    "\033[38;5;10m       .-MMMM\033[38;5;254m`..-:::::::-..`\033[38;5;10mMMMM-.\033[0m",
    "\033[38;5;10m     .:MMMM\033[38;5;254m.:MMMMMMMMMMMMMMM:.\033[38;5;10mMMMM:.\033[0m",
    "\033[38;5;10m    -MMM\033[38;5;254m-M---MMMMMMMMMMMMMMMMMMM.\033[38;5;10mMMM-\033[0m",
    "\033[38;5;10m  `:MMM\033[38;5;254m:MM`  :MMMM:....::-...-MMMM:\033[38;5;10mMMM:`\033[0m",
    "\033[38;5;10m  :MMM\033[38;5;254m:MMM`  :MM:`  ``    ``  `:MMM:\033[38;5;10mMMM:\033[0m",
    "\033[38;5;10m .MMM\033[38;5;254m.MMMM`  :MM.  -MM.  .MM-  `MMMM.\033[38;5;10mMMM.\033[0m",
    "\033[38;5;10m :MMM\033[38;5;254m:MMMM`  :MM.  -MM-  .MM:  `MMMM-\033[38;5;10mMMM:\033[0m",
    "\033[38;5;10m :MMM\033[38;5;254m:MMMM`  :MM.  -MM-  .MM:  `MMMM:\033[38;5;10mMMM:\033[0m",
    "\033[38;5;10m :MMM\033[38;5;254m:MMMM`  :MM.  -MM-  .MM:  `MMMM-\033[38;5;10mMMM:\033[0m",
    "\033[38;5;10m .MMM\033[38;5;254m.MMMM`  :MM:--:MM:--:MM:  `MMMM.\033[38;5;10mMMM.\033[0m",
    "\033[38;5;10m  :MMM\033[38;5;254m:MMM-  `-MMMMMMMMMMMM-`  -MMM-\033[38;5;10mMMM:\033[0m",
    "\033[38;5;10m   :MMM\033[38;5;254m:MMM:`                `:MMM:\033[38;5;10mMMM:\033[0m",
    "\033[38;5;10m    .MMM\033[38;5;254m.MMMM:--------------:MMMM.\033[38;5;10mMMM.\033[0m",
    "\033[38;5;10m      '-MMMM\033[38;5;254m.-MMMMMMMMMMMMMMM-.\033[38;5;10mMMMM-'\033[0m",
    "\033[38;5;10m        '.-MMMM\033[38;5;254m``--:::::--``\033[38;5;10mMMMM-.'\033[0m",
    "\033[38;5;10m            '-MMMMMMMMMMMMM-'\033[0m",
    "\033[38;5;10m               ``-:::::-``\033[0m"
};
static const int mint_logo_lines = 19;

static const char *nixos_logo[] = {
    "\033[38;5;33m          ▗▄▄▄       \033[38;5;110m▗▄▄▄▄    ▄▄▄▖\033[0m",
    "\033[38;5;33m          ▜███▙       \033[38;5;110m▜███▙  ▟███▛\033[0m",
    "\033[38;5;33m           ▜███▙       \033[38;5;110m▜███▙▟███▛\033[0m",
    "\033[38;5;33m            ▜███▙       \033[38;5;110m▜██████▛\033[0m",
    "\033[38;5;33m     ▟█████████████████▙ \033[38;5;110m▜████▛     \033[38;5;33m▟▙\033[0m",
    "\033[38;5;33m    ▟███████████████████▙ \033[38;5;110m▜███▙    \033[38;5;33m▟██▙\033[0m",
    "\033[38;5;110m           ▄▄▄▄▖           ▜███▙  \033[38;5;33m▟███▛\033[0m",
    "\033[38;5;110m          ▟███▛             ▜██▛ \033[38;5;33m▟███▛\033[0m",
    "\033[38;5;110m         ▟███▛               ▜▛ \033[38;5;33m▟███▛\033[0m",
    "\033[38;5;110m▟███████████▛                  \033[38;5;33m▟██████████▙\033[0m",
    "\033[38;5;110m▜██████████▛                  \033[38;5;33m▟███████████▛\033[0m",
    "\033[38;5;110m      ▟███▛ \033[38;5;33m▟▙               ▟███▛\033[0m",
    "\033[38;5;110m     ▟███▛ \033[38;5;33m▟██▙             ▟███▛\033[0m",
    "\033[38;5;110m    ▟███▛  \033[38;5;33m▜███▙           ▝▀▀▀▀\033[0m",
    "\033[38;5;110m    ▜██▛    \033[38;5;33m▜███▙ \033[38;5;110m▜██████████████████▛\033[0m",
    "\033[38;5;110m     ▜▛     \033[38;5;33m▟████▙ \033[38;5;110m▜████████████████▛\033[0m",
    "\033[38;5;33m           ▟██████▙       \033[38;5;110m▜███▙\033[0m",
    "\033[38;5;33m          ▟███▛▜███▙       \033[38;5;110m▜███▙\033[0m",
    "\033[38;5;33m         ▟███▛  ▜███▙       \033[38;5;110m▜███▙\033[0m",
    "\033[38;5;33m         ▝▀▀▀    ▀▀▀▀▘       \033[38;5;110m▀▀▘\033[0m"
};
static const int nixos_logo_lines = 20;

static const char *redhat_logo[] = {
    "\033[38;5;196m           .MMM..:MMMMMMM\033[0m",
    "\033[38;5;196m           MMMMMMMMMMMMMMMMMM\033[0m",
    "\033[38;5;196m           MMMMMMMMMMMMMMMMMMMM.\033[0m",
    "\033[38;5;196m          MMMMMMMMMMMMMMMMMMMMMM\033[0m",
    "\033[38;5;196m         ,MMMMMMMMMMMMMMMMMMMMMM:\033[0m",
    "\033[38;5;196m         MMMMMMMMMMMMMMMMMMMMMMMM\033[0m",
    "\033[38;5;196m   .MMMM'  MMMMMMMMMMMMMMMMMMMMMM\033[0m",
    "\033[38;5;196m  MMMMMM    `MMMMMMMMMMMMMMMMMMMM.\033[0m",
    "\033[38;5;196m MMMMMMMM      MMMMMMMMMMMMMMMMMM .\033[0m",
    "\033[38;5;196m MMMMMMMMM.       `MMMMMMMMMMMMM' MM.\033[0m",
    "\033[38;5;196m MMMMMMMMMMM.                     MMMM\033[0m",
    "\033[38;5;196m `MMMMMMMMMMMMM.                 ,MMMMM.\033[0m",
    "\033[38;5;196m  `MMMMMMMMMMMMMMMMM.          ,MMMMMMMM.\033[0m",
    "\033[38;5;196m     MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\033[0m",
    "\033[38;5;196m       MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM:\033[0m",
    "\033[38;5;196m          MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\033[0m",
    "\033[38;5;196m             `MMMMMMMMMMMMMMMMMMMMMMMM:\033[0m",
    "\033[38;5;196m                 ``MMMMMMMMMMMMMMMMM'\033[0m"
};
static const int redhat_logo_lines = 18;

static const char *linux_logo[] = {
    "\033[38;5;220m     .---.       \033[0m",
    "\033[38;5;220m    | o o |      \033[0m",
    "\033[38;5;220m    | \\_/ |      \033[0m",
    "\033[38;5;220m   .-'___'-.     \033[0m",
    "\033[38;5;220m  /  `   `  \\    \033[0m",
    "\033[38;5;220m |   |   |   |   \033[0m",
    "\033[38;5;220m  \\__\\___/__/    \033[0m"
};
static const int linux_logo_lines = 7;

/* Theme system */

typedef struct { const char *name, *border, *label; } Theme;

static const Theme themes[] = {
    { "nord",       "\033[38;5;110m", "\033[38;5;153m" },
    { "dracula",    "\033[38;5;141m", "\033[38;5;212m" },
    { "gruvbox",    "\033[38;5;172m", "\033[38;5;214m" },
    { "catppuccin", "\033[38;5;147m", "\033[38;5;183m" },
    { "amber",      "\033[38;5;136m", "\033[38;5;220m" },
};
static const int num_themes = 5;

static const Theme *find_theme(const char *name) {
    for (int i = 0; i < num_themes; i++)
        if (strcmp(themes[i].name, name) == 0) return &themes[i];
    return NULL;
}

/* Utility helpers */

static void trim_trailing(char *str) {
    size_t len = strlen(str);
    while (len > 0 && (isspace((unsigned char)str[len-1]) || str[len-1] == '\n' || str[len-1] == '\r'))
        str[--len] = '\0';
}

/* Visual width: strips ANSI escapes and counts UTF-8 start bytes only. */
static int visual_strlen(const char *str) {
    int len = 0;
    for (int i = 0; str[i]; ) {
        if (str[i] == '\033' || str[i] == '\x1B') {
            i++;
            if (str[i] == '[') {
                i++;
                while (str[i] && !isalpha((unsigned char)str[i])) i++;
                if (str[i]) i++;
            }
        } else {
            /* Count only UTF-8 lead bytes (not 10xxxxxx continuation bytes). */
            if (((unsigned char)str[i] & 0xC0) != 0x80) len++;
            i++;
        }
    }
    return len;
}

static void set_buf(char *buf, size_t max, const char *s) {
    snprintf(buf, max, "%s", s);
}

/* Build a 10-wide progress bar into out[].
   invert=0: lower pct is good (memory, disk).
   invert=1: higher pct is good (battery). */
static void build_bar(char *out, size_t out_len, double pct, int bar_width, int invert) {
    int filled = (int)((pct / 100.0) * bar_width);
    if (filled < 0) filled = 0;
    if (filled > bar_width) filled = bar_width;

    const char *bar_color = invert
        ? (pct > 50.0 ? "\033[38;5;110m" : pct > 20.0 ? "\033[38;5;179m" : "\033[38;5;167m")
        : (pct < 60.0 ? "\033[38;5;110m" : pct < 80.0 ? "\033[38;5;179m" : "\033[38;5;167m");

    char *bp = out;
    int room = (int)out_len;
    int w;
    if (filled > 0) {
        w = snprintf(bp, room, "%s", bar_color); bp += w; room -= w;
        for (int i = 0; i < filled; i++) { w = snprintf(bp, room, "━"); bp += w; room -= w; }
        w = snprintf(bp, room, "\033[0m"); bp += w; room -= w;
    }
    if (filled < bar_width) {
        w = snprintf(bp, room, "\033[38;5;238m"); bp += w; room -= w;
        for (int i = filled; i < bar_width; i++) { w = snprintf(bp, room, "━"); bp += w; room -= w; }
        snprintf(bp, room, "\033[0m");
    }
}

/* read /proc/meminfo in a single pass; returns 1 on success. */
static int read_meminfo_kb(long *total_kb, long *used_kb) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return 0;

    long total = -1, avail = -1, free_kb = -1, buffers = 0, cached = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if      (strncmp(line, "MemTotal:",     9)  == 0) sscanf(line + 9,  "%ld", &total);
        else if (strncmp(line, "MemAvailable:", 13) == 0) sscanf(line + 13, "%ld", &avail);
        else if (strncmp(line, "MemFree:",      8)  == 0) sscanf(line + 8,  "%ld", &free_kb);
        else if (strncmp(line, "Buffers:",      8)  == 0) sscanf(line + 8,  "%ld", &buffers);
        else if (strncmp(line, "Cached:",       7)  == 0) sscanf(line + 7,  "%ld", &cached);
    }
    fclose(fp);

    if (total < 0) return 0;
    /* MemAvailable is absent on kernels < 3.14; fall back to free+buffers+cached. */
    if (avail < 0) avail = (free_kb >= 0) ? free_kb + buffers + cached : total;
    long u = total - avail;
    *total_kb = total;
    *used_kb  = u < 0 ? 0 : u;
    return 1;
}

/* select an embedded logo by lowercase distro/override name. Returns NULL if no match. */
static const char **pick_logo(const char *lower, int *lines_out) {
#define LOGO(arr) do { *lines_out = arr##_lines; return arr; } while(0)
    if (strstr(lower, "arch"))   LOGO(arch_logo);
    if (strstr(lower, "debian")) LOGO(debian_logo);
    if (strstr(lower, "ubuntu")) LOGO(ubuntu_logo);
    if (strstr(lower, "fedora")) LOGO(fedora_logo);
    if (strstr(lower, "alpine")) LOGO(alpine_logo);
    if (strstr(lower, "kali"))   LOGO(kali_logo);
    if (strstr(lower, "gentoo")) LOGO(gentoo_logo);
    if (strstr(lower, "mint"))   LOGO(mint_logo);
    if (strstr(lower, "nixos"))  LOGO(nixos_logo);
    if (strstr(lower, "red hat") || strstr(lower, "redhat") ||
        strstr(lower, "rhel")    || strstr(lower, "centos")) LOGO(redhat_logo);
    if (strstr(lower, "linux"))  LOGO(linux_logo);
    if (strstr(lower, "default") || strstr(lower, "myfetch")) LOGO(default_ascii);
#undef LOGO
    return NULL;
}

/* System data functions */

static pid_t get_process_info(pid_t pid, char *comm, size_t comm_len) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    FILE *fp = fopen(path, "r");
    if (!fp) return -1;

    char buf[1024];
    int ok = (fgets(buf, sizeof(buf), fp) != NULL);
    fclose(fp);
    if (!ok) return -1;

    char *last_paren = strrchr(buf, ')');
    if (!last_paren) return -1;

    char *first_paren = strchr(buf, '(');
    if (first_paren && first_paren < last_paren) {
        size_t name_len = (size_t)(last_paren - (first_paren + 1));
        if (name_len >= comm_len) name_len = comm_len - 1;
        strncpy(comm, first_paren + 1, name_len);
        comm[name_len] = '\0';
    } else {
        set_buf(comm, comm_len, "unknown");
    }

    pid_t ppid = -1;
    char state;
    sscanf(last_paren + 1, " %c %d", &state, &ppid);
    return ppid;
}

static void get_terminal(char *buf, size_t max_len) {
    const char *tp = getenv("TERM_PROGRAM");
    if (tp) {
        if      (strcmp(tp, "vscode")        == 0) { set_buf(buf, max_len, "Visual Studio Code"); return; }
        else if (strcmp(tp, "Apple_Terminal") == 0) { set_buf(buf, max_len, "Apple Terminal");    return; }
        else if (strcmp(tp, "iTerm.app")      == 0) { set_buf(buf, max_len, "iTerm2");            return; }
        else if (strcmp(tp, "WarpTerminal")   == 0) { set_buf(buf, max_len, "Warp");              return; }
        else if (strcmp(tp, "WezTerm")        == 0) { set_buf(buf, max_len, "WezTerm");           return; }
        else if (strcmp(tp, "Tabby")          == 0) { set_buf(buf, max_len, "Tabby");             return; }
        else if (strcmp(tp, "Hyper")          == 0) { set_buf(buf, max_len, "Hyper");             return; }
        else                                        { set_buf(buf, max_len, tp);                  return; }
    }

    const char *te = getenv("TERMINAL_EMULATOR");
    if (te) { set_buf(buf, max_len, te); return; }
    if (getenv("KONSOLE_VERSION") || getenv("KONSOLE_INFO"))         { set_buf(buf, max_len, "Konsole");        return; }
    if (getenv("GNOME_TERMINAL_SCREEN") || getenv("GNOME_TERMINAL_SERVICE")) { set_buf(buf, max_len, "GNOME Terminal"); return; }
    if (getenv("MATE_TERMINAL_SCREEN"))                              { set_buf(buf, max_len, "MATE Terminal");  return; }
    if (getenv("KITTY_WINDOW_ID"))                                   { set_buf(buf, max_len, "kitty");          return; }
    if (getenv("ALACRITTY_WINDOW_ID") || getenv("ALACRITTY_LOG"))   { set_buf(buf, max_len, "Alacritty");      return; }
    if (getenv("WT_SESSION"))                                        { set_buf(buf, max_len, "Windows Terminal"); return; }

    const char *term = getenv("TERM");
    if (term) {
        if      (strcmp(term, "xterm-kitty") == 0) { set_buf(buf, max_len, "kitty");     return; }
        else if (strcmp(term, "alacritty")   == 0) { set_buf(buf, max_len, "Alacritty"); return; }
        else if (strcmp(term, "foot")        == 0) { set_buf(buf, max_len, "foot");      return; }
    }

    static const char *known_terminals[] = {
        "gnome-terminal", "gnome-terminal-", "gnome-terminal-server",
        "konsole", "konsolebin", "xfce4-terminal", "lxterminal",
        "mate-terminal", "terminator", "alacritty", "kitty",
        "wezterm", "wezterm-gui", "urxvt", "rxvt", "rxvt-unicode",
        "xterm", "st", "cool-retro-term", "guake", "tilda", "tilix",
        "terminology", "qterminal", "deepin-terminal", "pantheon-terminal",
        "foot", "blackbox", "tabby", "hyper", "rio", "contour", "ghostty",
        "footclient", NULL
    };
    static const char *skip_list[] = {
        "bash", "zsh", "sh", "fish", "orig-fish", "ksh", "dash", "csh", "tcsh",
        "sudo", "su", "doas", "systemd", "init", "login",
        "myfetch", "sh.exe", "bash.exe", NULL
    };

    char matched[256] = "";
    char fallback[256] = "";
    pid_t ppid = getppid();

    for (int depth = 0; depth < 10 && ppid > 1; depth++) {
        char comm[256] = "";
        pid_t next = get_process_info(ppid, comm, sizeof(comm));
        if (next <= 0 || ppid == next) break;

        int found = 0;
        for (int i = 0; known_terminals[i]; i++) {
            if (strcmp(comm, known_terminals[i]) == 0 || strstr(comm, known_terminals[i])) {
                snprintf(matched, sizeof(matched), "%s", comm);
                found = 1;
                break;
            }
        }
        if (found) break;

        if (fallback[0] == '\0') {
            int skip = 0;
            for (int i = 0; skip_list[i]; i++) {
                if (strcmp(comm, skip_list[i]) == 0) { skip = 1; break; }
            }
            if (!skip) snprintf(fallback, sizeof(fallback), "%s", comm);
        }
        ppid = next;
    }

    char *det = matched[0] ? matched : fallback;
    if (det[0]) {
        if      (strstr(det, "gnome-terminal"))                              set_buf(buf, max_len, "GNOME Terminal");
        else if (strcmp(det, "konsole")   == 0 || strcmp(det, "konsolebin") == 0) set_buf(buf, max_len, "Konsole");
        else if (strcmp(det, "xfce4-terminal") == 0)                         set_buf(buf, max_len, "XFCE Terminal");
        else if (strcmp(det, "mate-terminal")  == 0)                         set_buf(buf, max_len, "MATE Terminal");
        else if (strcmp(det, "lxterminal")     == 0)                         set_buf(buf, max_len, "LXTerminal");
        else if (strcmp(det, "urxvt")  == 0 || strcmp(det, "rxvt-unicode") == 0) set_buf(buf, max_len, "URxvt");
        else if (strcmp(det, "alacritty")  == 0)                             set_buf(buf, max_len, "Alacritty");
        else if (strcmp(det, "kitty")      == 0)                             set_buf(buf, max_len, "kitty");
        else if (strcmp(det, "wezterm") == 0 || strcmp(det, "wezterm-gui") == 0) set_buf(buf, max_len, "WezTerm");
        else if (strcmp(det, "terminator") == 0)                             set_buf(buf, max_len, "Terminator");
        else if (strcmp(det, "tilix")      == 0)                             set_buf(buf, max_len, "Tilix");
        else if (strcmp(det, "tilda")      == 0)                             set_buf(buf, max_len, "Tilda");
        else if (strcmp(det, "guake")      == 0)                             set_buf(buf, max_len, "Guake");
        else if (strcmp(det, "qterminal")  == 0)                             set_buf(buf, max_len, "QTerminal");
        else if (strcmp(det, "foot") == 0 || strcmp(det, "footclient") == 0) set_buf(buf, max_len, "foot");
        else if (strcmp(det, "xterm")      == 0)                             set_buf(buf, max_len, "Xterm");
        else if (strcmp(det, "ghostty")    == 0)                             set_buf(buf, max_len, "Ghostty");
        else {
            if (det[0] >= 'a' && det[0] <= 'z') det[0] = det[0] - 'a' + 'A';
            set_buf(buf, max_len, det);
        }
        return;
    }

    if (term) set_buf(buf, max_len, term);
    else      set_buf(buf, max_len, "Unknown");
}

static void get_os(char *buf, size_t max_len) {
    FILE *fp = fopen("/etc/os-release", "r");
    if (!fp) { set_buf(buf, max_len, "Linux"); return; }
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
            char *val = line + 12;
            if (*val == '"') val++;
            char *end = val + strlen(val) - 1;
            while (end >= val && (*end == '\n' || *end == '\r' || *end == '"')) *end-- = '\0';
            set_buf(buf, max_len, val);
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    set_buf(buf, max_len, "Linux");
}

static void get_kernel(char *buf, size_t max_len) {
    struct utsname u;
    if (uname(&u) == 0) snprintf(buf, max_len, "%s %s", u.sysname, u.release);
    else set_buf(buf, max_len, "Linux");
}

static void get_uptime(char *buf, size_t max_len) {
    FILE *fp = fopen("/proc/uptime", "r");
    if (!fp) { set_buf(buf, max_len, "Unknown"); return; }
    double secs;
    int ok = (fscanf(fp, "%lf", &secs) == 1);
    fclose(fp);
    if (!ok) { set_buf(buf, max_len, "Unknown"); return; }

    int days  = (int)(secs / 86400);
    int hours = (int)((secs - days * 86400) / 3600);
    int mins  = (int)((secs - days * 86400 - hours * 3600) / 60);

    if (days > 0)  snprintf(buf, max_len, "%d days, %d hours, %d mins", days, hours, mins);
    else if (hours > 0) snprintf(buf, max_len, "%d hours, %d mins", hours, mins);
    else snprintf(buf, max_len, "%d mins", mins);
}

static void get_shell(char *buf, size_t max_len) {
    char *path = getenv("SHELL");
    if (!path) {
        struct passwd *pw = getpwuid(getuid());
        path = pw ? pw->pw_shell : NULL;
    }
    if (!path) path = "/bin/sh";
    char *base = strrchr(path, '/');
    set_buf(buf, max_len, base ? base + 1 : path);
}

static void get_memory_bar(char *buf, size_t max_len) {
    long total_kb, used_kb;
    if (!read_meminfo_kb(&total_kb, &used_kb)) { set_buf(buf, max_len, "Unknown"); return; }

    double pct   = total_kb > 0 ? ((double)used_kb / total_kb) * 100.0 : 0.0;
    double total = total_kb / 1048576.0;
    double used  = used_kb  / 1048576.0;

    char bar[96];
    build_bar(bar, sizeof(bar), pct, 10, 0);
    snprintf(buf, max_len, "%s %.0f%% (%.2f / %.2f GiB)", bar, pct, used, total);
}

static void clean_cpu_string(char *cpu) {
    char *p;
    while ((p = strstr(cpu, "(R)"))   != NULL) memmove(p, p + 3, strlen(p + 3) + 1);
    while ((p = strstr(cpu, "(TM)"))  != NULL) memmove(p, p + 4, strlen(p + 4) + 1);
    while ((p = strstr(cpu, "CPU"))   != NULL) memmove(p, p + 3, strlen(p + 3) + 1);
    while ((p = strstr(cpu, "Processor")) != NULL) memmove(p, p + 9, strlen(p + 9) + 1);

    int i = 0, j = 0;
    char last = '\0';
    while (cpu[i]) {
        if (isspace((unsigned char)cpu[i])) {
            if (last != ' ' && j > 0) { cpu[j++] = ' '; last = ' '; }
        } else {
            cpu[j++] = cpu[i];
            last = cpu[i];
        }
        i++;
    }
    if (j > 0 && cpu[j-1] == ' ') cpu[j-1] = '\0';
    else                           cpu[j]   = '\0';
}

static void get_cpu(char *buf, size_t max_len) {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) { set_buf(buf, max_len, "Unknown"); return; }
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "model name", 10) == 0 || strncmp(line, "Model", 5) == 0) {
            char *val = strchr(line, ':');
            if (val) {
                val++;
                while (isspace((unsigned char)*val)) val++;
                char tmp[256];
                set_buf(tmp, sizeof(tmp), val);
                trim_trailing(tmp);
                clean_cpu_string(tmp);
                set_buf(buf, max_len, tmp);
                fclose(fp);
                return;
            }
        }
    }
    fclose(fp);
    set_buf(buf, max_len, "Unknown CPU");
}

static const char *get_distro_color(const char *distro) {
    char lower[256];
    set_buf(lower, sizeof(lower), distro);
    for (int i = 0; lower[i]; i++) lower[i] = tolower((unsigned char)lower[i]);

    if (strstr(lower, "ubuntu"))  return "\033[38;5;202m";
    if (strstr(lower, "debian"))  return "\033[38;5;161m";
    if (strstr(lower, "arch"))    return "\033[38;5;39m";
    if (strstr(lower, "fedora"))  return "\033[38;5;27m";
    if (strstr(lower, "mint"))    return "\033[38;5;10m";
    if (strstr(lower, "gentoo"))  return "\033[38;5;99m";
    if (strstr(lower, "alpine"))  return "\033[38;5;32m";
    if (strstr(lower, "kali"))    return "\033[38;5;32m";
    if (strstr(lower, "nixos"))   return "\033[38;5;110m";
    if (strstr(lower, "red hat") || strstr(lower, "rhel") ||
        strstr(lower, "centos")  || strstr(lower, "redhat")) return "\033[38;5;196m";
    return "\033[1;36m";
}

static void get_exe_directory(char *dir_buf, size_t max_len) {
    char path[512];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len != -1) {
        path[len] = '\0';
        char *slash = strrchr(path, '/');
        if (slash) {
            *slash = '\0';
            set_buf(dir_buf, max_len, path);
            return;
        }
    }
    set_buf(dir_buf, max_len, ".");
}

static char **load_ascii_file(const char *path, int *num_lines) {
    FILE *fp = fopen(path, "r");
    if (!fp) return NULL;

    char **lines = malloc(sizeof(char *) * 256);
    if (!lines) { fclose(fp); return NULL; }
    int count = 0;
    char buffer[512];

    while (fgets(buffer, sizeof(buffer), fp) && count < 256) {
        size_t len = strlen(buffer);
        while (len > 0 && (buffer[len-1] == '\n' || buffer[len-1] == '\r'))
            buffer[--len] = '\0';
        lines[count] = strdup(buffer);
        if (!lines[count]) break;
        count++;
    }
    fclose(fp);
    *num_lines = count;
    return lines;
}

static void free_ascii(char **lines, int num_lines) {
    if (!lines) return;
    for (int i = 0; i < num_lines; i++) free(lines[i]);
    free(lines);
}

static void get_battery(char *buf, size_t max_len) {
    static const char *bat_paths[] = {
        "/sys/class/power_supply/BAT0",
        "/sys/class/power_supply/BAT1",
        "/sys/class/power_supply/BAT2",
    };
    int capacity = -1;
    char status[64] = "";

    for (int d = 0; d < 3; d++) {
        char path[256];
        snprintf(path, sizeof(path), "%s/capacity", bat_paths[d]);
        FILE *fp = fopen(path, "r");
        if (!fp) continue;
        if (fscanf(fp, "%d", &capacity) != 1) { fclose(fp); capacity = -1; continue; }
        fclose(fp);
        snprintf(path, sizeof(path), "%s/status", bat_paths[d]);
        fp = fopen(path, "r");
        if (fp) { if (fgets(status, sizeof(status), fp)) trim_trailing(status); fclose(fp); }
        break;
    }

    if (capacity < 0) { buf[0] = '\0'; return; }

    char bar[96];
    build_bar(bar, sizeof(bar), (double)capacity, 10, 1);

    const char *suffix = "";
    if (strcmp(status, "Charging")     == 0) suffix = " Charging";
    else if (strcmp(status, "Not charging") == 0) suffix = " Plugged in";
    else if (strcmp(status, "Full")         == 0) suffix = " Full";

    snprintf(buf, max_len, "%s %d%%%s", bar, capacity, suffix);
}

static void get_gpu(char *buf, size_t max_len) {
    static const char *card_paths[] = {
        "/sys/class/drm/card0",
        "/sys/class/drm/card1",
        "/sys/class/drm/card2",
    };
    unsigned int vendor_id = 0, device_id = 0;

    for (int d = 0; d < 3; d++) {
        char path[256];
        snprintf(path, sizeof(path), "%s/device/vendor", card_paths[d]);
        FILE *fp = fopen(path, "r");
        if (!fp) continue;
        if (fscanf(fp, "%x", &vendor_id) != 1) { fclose(fp); vendor_id = 0; continue; }
        fclose(fp);
        snprintf(path, sizeof(path), "%s/device/device", card_paths[d]);
        fp = fopen(path, "r");
        if (fp) { fscanf(fp, "%x", &device_id); fclose(fp); }
        break;
    }

    if (!vendor_id) { buf[0] = '\0'; return; }

    unsigned int vid = vendor_id & 0xFFFF;
    unsigned int did = device_id & 0xFFFF;

    char vendor_short[16] = "";
    if      (vid == 0x1002) set_buf(vendor_short, sizeof(vendor_short), "AMD");
    else if (vid == 0x10de) set_buf(vendor_short, sizeof(vendor_short), "NVIDIA");
    else if (vid == 0x8086) set_buf(vendor_short, sizeof(vendor_short), "Intel");
    else                    snprintf(vendor_short, sizeof(vendor_short), "%04x", vid);

    static const char *pci_ids_paths[] = {
        "/usr/share/hwdata/pci.ids",
        "/usr/share/misc/pci.ids",
        "/usr/share/pci.ids",
    };
    char device_name[256] = "";
    char target_vendor[8];
    snprintf(target_vendor, sizeof(target_vendor), "%04x", vid);

    for (int p = 0; p < 3 && !device_name[0]; p++) {
        FILE *fp = fopen(pci_ids_paths[p], "r");
        if (!fp) continue;
        char line[512];
        int in_vendor = 0;
        while (fgets(line, sizeof(line), fp)) {
            if (line[0] == '#' || line[0] == '\r' || line[0] == '\n') continue;
            if (!in_vendor) {
                if (strncmp(line, target_vendor, 4) == 0 && (line[4] == ' ' || line[4] == '\t'))
                    in_vendor = 1;
            } else {
                if (line[0] != '\t') break;
                if (line[1] != '\t') {
                    unsigned int line_did = 0;
                    if (sscanf(line + 1, "%x", &line_did) == 1 && line_did == did) {
                        char *name = line + 5;
                        while (*name == ' ' || *name == '\t') name++;
                        snprintf(device_name, sizeof(device_name), "%.*s",
                                 (int)(sizeof(device_name) - 1), name);
                        trim_trailing(device_name);
                        break;
                    }
                }
            }
        }
        fclose(fp);
    }

    if (device_name[0]) snprintf(buf, max_len, "%s", device_name);
    else                snprintf(buf, max_len, "%s [%04x]", vendor_short, did);
}

static void get_cpu_temp(char *buf, size_t max_len) {
    for (int i = 0; i < 10; i++) {
        char path[256];
        snprintf(path, sizeof(path), "/sys/class/thermal/thermal_zone%d/temp", i);
        FILE *fp = fopen(path, "r");
        if (!fp) continue;
        int millic = 0;
        if (fscanf(fp, "%d", &millic) == 1 && millic > 0) {
            snprintf(buf, max_len, "%d°C", millic / 1000);
            fclose(fp);
            return;
        }
        fclose(fp);
    }
    buf[0] = '\0';
}

static void get_packages(char *buf, size_t max_len) {
    int count = 0;
    DIR *dir = opendir("/var/lib/pacman/local");
    if (dir) {
        struct dirent *e;
        while ((e = readdir(dir))) if (e->d_name[0] != '.') count++;
        closedir(dir);
        if (count > 0) { snprintf(buf, max_len, "%d (pacman)", count); return; }
    }

    dir = opendir("/var/lib/dpkg/info");
    if (dir) {
        struct dirent *e;
        while ((e = readdir(dir))) {
            size_t len = strlen(e->d_name);
            if (len > 5 && strcmp(e->d_name + len - 5, ".list") == 0) count++;
        }
        closedir(dir);
        if (count > 0) { snprintf(buf, max_len, "%d (dpkg)", count); return; }
    }

    FILE *fp = fopen("/lib/apk/db/installed", "r");
    if (!fp) fp = fopen("/var/lib/apk/db/installed", "r");
    if (fp) {
        char line[256];
        while (fgets(line, sizeof(line), fp)) if (strncmp(line, "P:", 2) == 0) count++;
        fclose(fp);
        if (count > 0) { snprintf(buf, max_len, "%d (apk)", count); return; }
    }

    buf[0] = '\0';
}

static void get_disk(char *buf, size_t max_len) {
    struct statvfs vfs;
    if (statvfs("/", &vfs) != 0) { buf[0] = '\0'; return; }

    unsigned long long total = (unsigned long long)vfs.f_blocks * vfs.f_frsize;
    unsigned long long free  = (unsigned long long)vfs.f_bfree  * vfs.f_frsize;
    unsigned long long used  = total - free;

    if (!total) { buf[0] = '\0'; return; }

    double pct   = ((double)used / total) * 100.0;
    double total_gib = total / 1073741824.0;
    double used_gib  = used  / 1073741824.0;

    char bar[96];
    build_bar(bar, sizeof(bar), pct, 10, 0);
    snprintf(buf, max_len, "%s %.0f%% (%.1f / %.1f GiB)", bar, pct, used_gib, total_gib);
}

static void print_help(const char *prog) {
    printf("\033[1;36m%s\033[0m - A retro distro fetch utility. ( fastfetch is still better lol )\n\n", prog);
    printf("Usage: %s [options]\n\n", prog);
    printf("Options:\n");
    printf("  -a, --ascii <file>      Path to custom ASCII art file\n");
    printf("  -f, --force-logo <name> Force a specific built-in distro logo\n");
    printf("                          (arch, debian, ubuntu, fedora, alpine, kali, gentoo, mint, nixos, redhat, linux, default)\n");
    printf("  -t, --theme <name>      Color theme for borders and labels\n");
    printf("                          (nord, dracula, gruvbox, catppuccin, amber, default)\n");
    printf("  -n, --no-center-v       Disable full-screen vertical centering\n");
    printf("  -h, --help              Display this help menu\n\n");
}

/* main babe */

int main(int argc, char **argv) {
    char *custom_ascii_path = NULL;
    char *force_logo_name   = NULL;
    char *theme_name        = NULL;
    int   center_v          = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--ascii") == 0) {
            if (i + 1 < argc) custom_ascii_path = argv[++i];
            else { fprintf(stderr, "Error: %s requires a file path.\n", argv[i]); return 1; }
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--force-logo") == 0) {
            if (i + 1 < argc) force_logo_name = argv[++i];
            else { fprintf(stderr, "Error: %s requires a logo name.\n", argv[i]); return 1; }
        } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--theme") == 0) {
            if (i + 1 < argc) theme_name = argv[++i];
            else { fprintf(stderr, "Error: %s requires a theme name.\n", argv[i]); return 1; }
        } else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--no-center-v") == 0) {
            center_v = 0;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]); return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_help(argv[0]); return 1;
        }
    }

    char os_name[256];
    get_os(os_name, sizeof(os_name));

    /* Load ASCII art */
    char **ascii_lines = NULL;
    int num_ascii_lines = 0;
    int is_embedded = 0;

    if (custom_ascii_path) {
        ascii_lines = load_ascii_file(custom_ascii_path, &num_ascii_lines);
        if (!ascii_lines)
            fprintf(stderr, "Warning: Could not open '%s'. Using fallback.\n", custom_ascii_path);
    }

    if (!ascii_lines && force_logo_name) {
        is_embedded = 1;
    }
    /* ~/.config/myfetch/ascii.txt — user config, highest file priority */
    if (!ascii_lines && !is_embedded) {
        const char *home = getenv("HOME");
        if (!home) {
            struct passwd *pw = getpwuid(getuid());
            if (pw) home = pw->pw_dir;
        }
        if (home) {
            char config_path[1024];
            snprintf(config_path, sizeof(config_path), "%s/.config/myfetch/ascii.txt", home);
            ascii_lines = load_ascii_file(config_path, &num_ascii_lines);
        }
    }
    if (!ascii_lines && !is_embedded) {
        ascii_lines = load_ascii_file("ascii.txt", &num_ascii_lines);
    }
    if (!ascii_lines && !is_embedded) {
        char exe_dir[512], local_path[1024];
        get_exe_directory(exe_dir, sizeof(exe_dir));
        snprintf(local_path, sizeof(local_path), "%s/ascii.txt", exe_dir);
        ascii_lines = load_ascii_file(local_path, &num_ascii_lines);
    }
    /* /usr/share/myfetch/ascii.txt — system-installed default art */
    if (!ascii_lines && !is_embedded) {
        ascii_lines = load_ascii_file("/usr/share/myfetch/ascii.txt", &num_ascii_lines);
    }
    if (!ascii_lines)
        is_embedded = 1;

    /* Select embedded logo */
    const char **embedded_logo       = default_ascii;
    int          embedded_logo_lines = default_ascii_lines;

    if (is_embedded) {
        const char *match_name = force_logo_name ? force_logo_name : os_name;
        char lower[256];
        set_buf(lower, sizeof(lower), match_name);
        for (int i = 0; lower[i]; i++) lower[i] = tolower((unsigned char)lower[i]);

        const char **logo = pick_logo(lower, &embedded_logo_lines);
        if (!logo && force_logo_name) {
            /* Invalid -f name: warn and fall back to auto-detected OS logo. */
            fprintf(stderr,
                "Warning: Unknown forced logo '%s'. "
                "Supported: arch, debian, ubuntu, fedora, alpine, kali, gentoo, mint, nixos, redhat, linux, default. "
                "Falling back.\n", force_logo_name);
            char os_lower[256];
            set_buf(os_lower, sizeof(os_lower), os_name);
            for (int i = 0; os_lower[i]; i++) os_lower[i] = tolower((unsigned char)os_lower[i]);
            logo = pick_logo(os_lower, &embedded_logo_lines);
        }
        if (logo) embedded_logo = logo;
        num_ascii_lines = embedded_logo_lines;
    }

    /* Gather system info */
    char username[256], hostname[256];
    char kernel[256], uptime[256], shell[256], terminal[256], cpu[256];
    char memory_bar[256], battery[256], gpu[256], packages[256], disk[256];
    char cpu_temp[64];

    char *user_env = getenv("USER");
    if (user_env) {
        set_buf(username, sizeof(username), user_env);
    } else {
        struct passwd *pw = getpwuid(getuid());
        set_buf(username, sizeof(username), pw ? pw->pw_name : "user");
    }
    if (gethostname(hostname, sizeof(hostname)) != 0)
        set_buf(hostname, sizeof(hostname), "linux");

    get_kernel(kernel, sizeof(kernel));
    get_uptime(uptime, sizeof(uptime));
    get_shell(shell, sizeof(shell));
    get_terminal(terminal, sizeof(terminal));
    get_cpu(cpu, sizeof(cpu));
    get_memory_bar(memory_bar, sizeof(memory_bar));
    get_battery(battery, sizeof(battery));
    get_gpu(gpu, sizeof(gpu));
    get_cpu_temp(cpu_temp, sizeof(cpu_temp));
    get_packages(packages, sizeof(packages));
    get_disk(disk, sizeof(disk));

    const char *reset  = "\033[0m";

    const char *border_color, *label_color;
    if (theme_name && strcmp(theme_name, "default") != 0) {
        const Theme *t = find_theme(theme_name);
        if (!t) {
            fprintf(stderr, "Warning: Unknown theme '%s'. "
                    "Available: nord, dracula, gruvbox, catppuccin, amber, default. "
                    "Using default.\n", theme_name);
            border_color = label_color = get_distro_color(os_name);
        } else {
            border_color = t->border;
            label_color  = t->label;
        }
    } else {
        border_color = label_color = get_distro_color(os_name);
    }

    /* Build info rows */
    char raw_info[14][512];
    char cpu_display[384];

    if (cpu_temp[0])
        snprintf(cpu_display, sizeof(cpu_display), "%s (%s)", cpu, cpu_temp);
    else
        snprintf(cpu_display, sizeof(cpu_display), "%s", cpu);

#define ROW(dst, key, val) \
    snprintf(dst, sizeof(dst), "%s" key "%s%s│%s %s", label_color, reset, border_color, reset, val)

    ROW(raw_info[0], "OS     ", os_name);
    ROW(raw_info[1], "Kernel ", kernel);
    ROW(raw_info[2], "Uptime ", uptime);
    ROW(raw_info[3], "Shell  ", shell);
    ROW(raw_info[4], "Term   ", terminal);
    ROW(raw_info[5], "CPU    ", cpu_display);
    int num_raw = 6;

    if (gpu[0])      { ROW(raw_info[num_raw], "GPU    ", gpu);      num_raw++; }
    if (packages[0]) { ROW(raw_info[num_raw], "Pkgs   ", packages); num_raw++; }
    ROW(raw_info[num_raw], "Memory ", memory_bar); num_raw++;
    if (disk[0])     { ROW(raw_info[num_raw], "Disk   ", disk);     num_raw++; }
    if (battery[0])  { ROW(raw_info[num_raw], "Battery", battery);  num_raw++; }

#undef ROW

    /* calculate card width */
    int max_content_w = 0;
    for (int i = 0; i < num_raw; i++) {
        int w = visual_strlen(raw_info[i]);
        if (w > max_content_w) max_content_w = w;
    }
    int header_w = (int)(strlen(username) + 1 + strlen(hostname));
    if (header_w + 4 > max_content_w) max_content_w = header_w + 4;
    int card_w = max_content_w;

    char info_lines[18][2048];

    /* Top border */
    char *p = info_lines[0];
    p += sprintf(p, "%s╭", border_color);
    for (int i = 0; i < card_w + 2; i++) p += sprintf(p, "─");
    sprintf(p, "╮%s", reset);

    /* Header row */
    int pad_l = (card_w - header_w) / 2;
    int pad_r = card_w - header_w - pad_l;
    p = info_lines[1];
    p += sprintf(p, "%s│ %s", border_color, reset);
    for (int i = 0; i < pad_l; i++) *p++ = ' ';
    p += sprintf(p, "\033[1m%s%s%s@\033[1m%s%s%s", label_color, username, reset, label_color, hostname, reset);
    for (int i = 0; i < pad_r; i++) *p++ = ' ';
    sprintf(p, " %s│%s", border_color, reset);

    /* Divider */
    p = info_lines[2];
    p += sprintf(p, "%s├", border_color);
    for (int i = 0; i < card_w + 2; i++) p += sprintf(p, "─");
    sprintf(p, "┤%s", reset);

    /* Detail rows */
    for (int i = 0; i < num_raw; i++) {
        int pad = card_w - visual_strlen(raw_info[i]);
        char row[2048];
        p = row;
        int room = sizeof(row);
        int w = snprintf(p, room, "%s│ %s%s", border_color, reset, raw_info[i]); p += w; room -= w;
        for (int j = 0; j < pad && room > 10; j++) { *p++ = ' '; room--; }
        snprintf(p, room, " %s│%s", border_color, reset);
        snprintf(info_lines[3 + i], sizeof(info_lines[3 + i]), "%s", row);
    }

    /* Bottom border */
    p = info_lines[3 + num_raw];
    p += sprintf(p, "%s╰", border_color);
    for (int i = 0; i < card_w + 2; i++) p += sprintf(p, "─");
    sprintf(p, "╯%s", reset);

    int num_info_lines = 4 + num_raw;

    /* calculate horizontal centers */
    int max_ascii_w = 0;
    for (int i = 0; i < num_ascii_lines; i++) {
        const char *line = is_embedded ? embedded_logo[i] : ascii_lines[i];
        int w = visual_strlen(line);
        if (w > max_ascii_w) max_ascii_w = w;
    }
    int max_info_w = 0;
    for (int i = 0; i < num_info_lines; i++) {
        int w = visual_strlen(info_lines[i]);
        if (w > max_info_w) max_info_w = w;
    }

    /* terminal size */
    struct winsize winsz;
    int cols = 80, rows = 24;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsz) == 0) {
        if (winsz.ws_col > 0) cols = winsz.ws_col;
        if (winsz.ws_row > 0) rows = winsz.ws_row;
    }

    int total_lines  = num_ascii_lines + 1 + num_info_lines;
    int top_padding  = 0;
    int bot_padding  = 0;
    if (center_v && rows > total_lines) {
        top_padding = (rows - total_lines) / 2;
        bot_padding = rows - total_lines - top_padding - 1; /* dont ask about the -1 idk either it looks better*/
        if (bot_padding < 0) bot_padding = 0;
    }

    int pad_ascii = (cols - max_ascii_w) / 2; if (pad_ascii < 0) pad_ascii = 0;
    int pad_info  = (cols - max_info_w)  / 2; if (pad_info  < 0) pad_info  = 0;
    int pad_blocks = (cols - 24) / 2;         if (pad_blocks < 0) pad_blocks = 0;

    /* render */
    if (center_v && rows > total_lines) {
        printf("\033[H\033[2J");
        for (int i = 0; i < top_padding; i++) printf("\n");
    }

    for (int i = 0; i < num_ascii_lines; i++) {
        const char *line = is_embedded ? embedded_logo[i] : ascii_lines[i];
        for (int j = 0; j < pad_ascii; j++) printf(" ");
        printf("%s\n", line);
    }
    printf("\n");

    for (int i = 0; i < num_info_lines; i++) {
        for (int j = 0; j < pad_info; j++) printf(" ");
        printf("%s\n", info_lines[i]);
    }

    for (int j = 0; j < pad_blocks; j++) printf(" ");
    printf("\033[40m   \033[41m   \033[42m   \033[43m   \033[44m   \033[45m   \033[46m   \033[47m   %s\n", reset);

    if (center_v && rows > total_lines)
        for (int i = 0; i < bot_padding; i++) printf("\n");

    free_ascii(ascii_lines, num_ascii_lines);
    return 0;
}
