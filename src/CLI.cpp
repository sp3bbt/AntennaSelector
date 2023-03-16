#include "CLI.h"
#ifdef ENABLE_CLI
// Include Library
#include <SimpleCLI.h>
// Create CLI Object
Gnamespace {

SimpleCLI cli;

// Commands
Command help;
Command list;
Command set;

// Callback in case of an error
void errorCallback(cmd_error* e)
{
    CommandError cmdError(e);// Create wrapper object
    auto error = cmdError.toString();
    c_error("%s", error.c_str());
}

// Callback function for ping command
void helpCallback(cmd* ) {
    c_println("usage:");
    c_println("\thelp");
    c_println("\tlist");
    c_println("\tset -i <1..6> -n \"nazwa\" -d \"opis\" -e <1|0>");
}
// set -i 1 -n "Nazwa anteny" -d "Opis anteny"
void listCallback(cmd* ) {
    for (int i = 0; i < 6; ++i) {
        c_printf("%d: %16s %16s %s\n", (i+1), config.antennas[i].name, config.antennas[i].desc, ((config.enable & (1 << i)) != 0) ? "On": "Off");
    }
}

void setCallback(cmd* c)
{
    Command cmd(c);// Create wrapper object

    // Get arguments
    Argument i = cmd.getArgument("i");
    Argument n = cmd.getArgument("n");
    Argument d = cmd.getArgument("d");
    Argument e = cmd.getArgument("e");

    // Get values
    int index = i.getValue().toInt() - 1;
    if (index < 0 || index > 5)
        return;

    {
        String s = n.getValue();
        if (s.length() > 0) {
            memset(config.antennas[index].name, 0, sizeof(config.antennas[index].name));
            strncpy(config.antennas[index].name, s.c_str(), sizeof(config.antennas[i].name) - 1);
        }
    }
    {
        String s = d.getValue();
        if (s.length() > 0) {
            memset(config.antennas[index].desc, 0, sizeof(config.antennas[index].desc));
            strncpy(config.antennas[index].desc, s.c_str(), sizeof(config.antennas[i].desc) - 1);
        }
    }
    if (e.isSet()) {
        int en = e.getValue().toInt();
        if (en) {
            config.enable |= (1 << (index));
        } else {
            config.enable &= ~(1 << (index));
        }
    }
}
} // namespace
#endif // ENABLE_CLI


void CLIsetup()
{
#ifdef ENABLE_CLI
    cli.setOnError(errorCallback);// Set error Callback
    help = cli.addCommand("help", helpCallback);
    list = cli.addCommand("list", listCallback);
    set = cli.addCommand("set", setCallback);
    set.addArgument("i");
    set.addArgument("n", "");
    set.addArgument("d", "");
    set.addArgument("e", "1");
#endif // ENABLE_CLI
}


void CLIreadSerial()
{
#ifdef ENABLE_CLI
    static int cliindex = 0;
    static char cliline[64];
    while (Serial.available() > 0) {
        char c = Serial.read();
        c_print(c);
        if (c == '\n') {
            cliline[cliindex] = '\0';
            cliindex = 0;
            if (strlen(cliline) > 0) {
                c_logf("got line: ");
                c_logf("%s", cliline);
                cli.parse(cliline);

                if (cli.errored()) {
                    CommandError cmdError = cli.getError();
                    auto error = cmdError.toString();
                    c_error("%s", error.c_str());
                }
            } else {
                c_debug("empty line");
            }
        } else if (isprint(c)) {
            cliline[cliindex++] = c;

            if (cliindex == (sizeof(cliline) - 1)) {
                cliline[cliindex] = '\0';
                cliindex = 0;
                c_print("line is too long: \"");
                c_print(cliline);
                c_print("\"");
            }
        }
    }
#endif // ENABLE_CLI
}
