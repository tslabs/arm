
#define X_M 30

void chip_msg()
{
	xy(10,22);
	
	if (ay_fnd)
	{
		color(C_INFO);
		msg("Chip F/W revision: 1.0");
	}
	else
	{
		color(C_WARN);
		msg("Chip NOT found! DEMO mode");
	}
}
	
void msg_main()
{
	cls();
	color(C_HEAD); xy(40,1);
    msg("AY-ARM Configuration Utility");
	color(C_NORM); xy(X_M,5);
    msg("1. Device info");
	color(C_NORM); xy(X_M,7);
    msg("2. AY Settings");
	color(C_NORM); xy(X_M,9);
    msg("3. SSG Settings");
	color(C_NORM); xy(X_M,14);
    msg("0. Save Settings");
	color(C_NORM); xy(X_M,16);
    msg("U. Upload Firmware");
	chip_msg();	
}

void msg_info()
{
	cls();
	color(C_HEAD); xy(100,1);
    msg("Chip Info:");
}
void msg_ay()
{
	cls();
	color(C_HEAD); xy(50,1);
    msg("AY-Chip Emulation Settings:");
}

void msg_ssg()
{
	cls();
	color(C_HEAD); xy(50,1);
    msg("Sound Generator Settings:");
}

void msg_save()
{
	fade();
	frame (32, 6, 30, 6, C_FRAM);
	color(C_QUST); xy(60, 8);
    msg("You are about to write");
	xy(70, 9);
    msg("settings into Flash");
	xy(98, 11); color(C_NORM);
    msg("Sure? (Y/N)");
}

void msg_save1()
{
	frame (48, 15, 24, 1, C_FRAM);
	color(C_INFO); xy(74, 16);
    msg("Saving ... ");
    msg(" DONE!");
	// color(C_WARN); msg("ERROR!");
}

void msg_fupd()
{
	fade();
	frame (32, 6, 30, 6, C_FRAM);
	color(C_QUST); xy(60, 8);
    msg("You are about to flash");
	color (C_WARN); xy(60, 9);
    msg("FIRMWARE ");
	color (C_QUST);
	msg("into the chip");
	xy(98, 11); color(C_NORM);
    msg("Sure? (Y/N)");
}

void msg_fupd1()
{
	frame (40, 15, 27, 1, C_FRAM);
	color(C_INFO); xy(60, 16);
    msg("Updating F/W ... ");
    msg(" DONE!");
	// color(C_WARN); msg("ERROR!");
}
