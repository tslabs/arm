#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TYPE_UNKNOWN	0
#define	TYPE_SCL	1
#define	TYPE_TRD	2
#define TYPE_TAP	3

#ifdef _WIN32
#define SLASH '\\'
#else
#define SLASH '/'
#endif

// structures

typedef struct {
	char name[10];
	char ext[3];
} xName;

struct {
	unsigned isBasic:1;
	unsigned headLess:1;
	unsigned asHobeta:1;
	unsigned mono:1;
	int aStart;
} opt;

#pragma pack(push,1)

typedef struct {
	char type;
	char name[10];
	unsigned char dll;
	unsigned char dlh;
	unsigned char asl;
	unsigned char ash;
	unsigned char pll;
	unsigned char plh;
} tHead;

typedef struct {
	char name[8];
	char ext;
	unsigned char stl;
	unsigned char sth;
	unsigned char lnl;
	unsigned char lnh;
	unsigned char slen;
	unsigned char sec;
	unsigned char trk;
} trdFile;

#pragma pack(pop)

// common

int readfile(char* path, char* buf, int maxlen) {
	FILE *infile = fopen(path,"rb");
	if (!infile) {
		printf("Can't open file '%s'\n",path);
		return -1;		// failed to open file
	}
	fseek(infile, 0, SEEK_END);
	int iflen = ftell(infile);
	rewind(infile);
	if (iflen <= maxlen) {
		fread(buf, iflen, 1, infile);
	} else {
		printf("File '%s' is too long (max is 0x%X bytes)\n", path, maxlen);
		iflen = -2;
	}
	fclose(infile);
	return iflen;
}

int savefile(char* path, char* buf, int len) {
	FILE *file = fopen(path, "wb");
	if (!file) {
		printf("Can't open file '%s' for writing\n",path);
		return 0;
	}
	if (buf && (len > 0)) fwrite(buf, len, 1, file);
	fclose(file);
	return 1;
}

int testExt(char* path, char* ext) {
	char* ptr = strrchr(path, '.');
	if (!ptr) return 0;
	char buf[255];
	strcpy(buf, ptr + 1);
	ptr = buf;
	while (*ptr) {
		*ptr = tolower(*ptr);
		ptr++;
	}
	if (strcmp(ext, buf)) return 0;
	return 1;
}

int getFileType(char* path) {
	FILE* file = fopen(path, "rb");
	char buf[9];
	int res = TYPE_UNKNOWN;
	if (testExt(path, "tap")) {
		res = TYPE_TAP;
	} else if (testExt(path, "scl") && file) {
		fread(buf, 9, 1, file);
		if ((strncmp(buf, "SINCLAIR", 8) == 0) && ((unsigned)buf[8] < 129))
			res = TYPE_SCL;
	} else if (testExt(path, "trd") && file) {
		fseek(file, 0, SEEK_END);
		size_t size = ftell(file);
		fseek(file, 0x8e7, SEEK_SET);
		int type = fgetc(file);
		if ((size > 0x8ff) && (size < 0xa0001) && (type == 0x10))
			res = TYPE_TRD;
	}
	if (file) fclose(file);
	return res;
}

xName cutName(char* path) {
	char fnam[strlen(path) + 1];
	char* ptr = strrchr(path, SLASH);		// last slash
	if (ptr) {
		strcpy(fnam, ptr + 1);
	} else {
		strcpy(fnam, path);
	}
	ptr = strrchr(fnam, '.');
	xName nm;
	memset(nm.name, 0x20, 10);
	memset(nm.ext, 0x20, 3);
	if (ptr) {
		*ptr = 0x00;
		memcpy(nm.ext, ptr + 1, (strlen(ptr + 1) > 3) ? 3 : strlen(ptr + 1));
	}
	memcpy(nm.name, fnam, (strlen(fnam) > 10) ? 10 : strlen(fnam));
	return nm;
}

int stdAccumulate(unsigned char* ptr, int size, int res) {
	while (size > 0) {
		res += *ptr;
		ptr++;
		size--;
	}
	return res;
}

int fgeti(FILE* file) {
	int res = fgetc(file) & 0xff;
	res |= (fgetc(file) << 8);
	res |= (fgetc(file) << 16);
	res |= (fgetc(file) << 24);
	return res;
}

void fputi(FILE* file, int val) {
	fputc(val & 0xff, file);
	fputc((val >> 8) & 0xff, file);
	fputc((val >> 16) & 0xff, file);
	fputc((val >> 24) & 0xff, file);	
}

// disk common

void makeName(char* name, char* buf) {
	memset(buf, 0x20, 9);
	char* ptr = strrchr(name, '.');
	if (ptr) {
		memcpy(buf, name, ((ptr - name) > 8) ? 8 : (ptr - name));
		ptr++;
		if (*ptr) buf[8] = *ptr;
	} else {
		memcpy(buf, name, (strlen(name) > 8) ? 8 : strlen(name));
	}
}

int getSum(char* data, int len) {
	unsigned int crc = 0;
	while (len--) {
		crc += (unsigned char)*data;
		data++;
	}
	return crc;
}

trdFile makeDSC(char* path, int len) {
	trdFile hd;
	xName nm = cutName(path);
	memcpy(hd.name, nm.name, 8);
	hd.lnl = len & 0xff;
	hd.lnh = (len >> 8) & 0xff;
	if (opt.isBasic) {
		hd.ext = 'B';
		hd.stl = hd.lnl;
		hd.sth = hd.lnh;
		len += 4;		// 80,AA,<AS.line>
	} else {
		hd.ext = nm.ext[0];
		hd.stl = nm.ext[1];
		hd.sth = nm.ext[2];
	}
	hd.slen = ((len >> 8) & 0xff) + ((len & 0xff) ? 1 : 0);
	return hd;
}

trdFile prepareFile(char* path, char* buf) {
	memset(buf, 0x00, 0x10000);
	trdFile hd;
	int ilen;
	memset(hd.name, 0x00, sizeof(trdFile));
	if (opt.asHobeta) {
		ilen = readfile(path, buf, 0xff11);	// +17 bytes header
		if (ilen > 0) {
			if (!buf[13])
				buf[13] = buf[14];
			memcpy((char*)&hd, buf, 15);
			memcpy(buf, buf+17, ilen-17);
		}
	} else {
		ilen = readfile(path, buf, 0xff00);
		if (ilen > 0) {
			if (opt.isBasic) {
				buf[ilen] = 0x80;
				buf[ilen+1] = 0xaa;
				buf[ilen+2] = opt.aStart & 0xff;
				buf[ilen+3] = (opt.aStart >> 8) & 0xff;
			}
			hd = makeDSC(path, ilen);
		}
	}
	return hd;
}

int saveoutput(char* oname, char* buf, trdFile hd) {
	int len;
	if ((hd.lnh + (hd.lnl ? 1 : 0)) == hd.slen) {
		len = hd.lnl | (hd.lnh << 8);
	} else {
		len = hd.slen << 8;
	}
	int res;
	if (opt.asHobeta) {
		char hbuf[0x10010];
		memcpy(hbuf, (char*)&hd, 13);
		hbuf[13] = 0x00;
		hbuf[14] = hd.slen;
		int crc = ((105 + 257 * stdAccumulate((unsigned char*)hbuf, 15, 0)) & 0xffff);
		hbuf[15] = crc & 0xff;
		hbuf[16] = ((crc & 0xff00) >> 8);
		memcpy(hbuf + 17, buf, len);
		res = savefile(oname, hbuf, len + 17);
	} else {
		res = savefile(oname, buf, len);
	}
	return res;
}

// tap

void tapAddBlock(FILE* file, int len, char* buf, int type) {
	int crc = type & 0xff;
	for (int i = 0; i < len; i++) {
		crc ^= buf[i];
	}
	fputc((len + 2) & 0xff, file);
	fputc(((len + 2) >> 8) & 0xff, file);
	fputc(type, file);
	fwrite(buf, len, 1, file);
	fputc(crc, file);
}

void tapCreate(char* path) {
	savefile(path, NULL, 0);
}

void tapList(char* path) {
	FILE* file = fopen(path, "rb");
	if (!file) {
		printf("Can't open image '%s'\n",path);
		return;
	}
	int len;
	tHead hd;
	int blk = 0;
	printf("Name\t\tType\tStart\tSize\n");
	printf("------------------------------------\n");
	while (1) {
		len = fgetc(file);
		len |= (fgetc(file) << 8);
		if (feof(file)) break;
		int type = fgetc(file);
		if (type == 0x00) {
			fread((char*)&hd, sizeof(tHead), 1, file);
			printf("%.10s\t%s\t%i\t%i\n",hd.name, hd.type ? "Bytes" : "Prog", hd.asl | (hd.ash << 8), hd.dll | (hd.dlh << 8));
			fgetc(file);		// skip crc
		} else {
			fseek(file, len - 1, SEEK_CUR);
			printf("blk-%.3i\t\t\t\t%i\n",blk,len - 2);
		}
		blk++;
	}
	fclose(file);
}

void tapPush(char* iname, char* fname) {
	char buf[0x10000];
	int flen = readfile(fname, buf, 0xfffd);
	if (flen < 0) return;	
	FILE* file = fopen(iname, "ab");
	if (!file) {
		printf("Can't open '%s' for append\n", iname);
		return;
	}
	if (!opt.headLess) {
		tHead hd;
		memset(hd.name, 0x20, 10);
		xName nm = cutName(fname);
		memcpy(hd.name, nm.name, 10);
		hd.dll = flen & 0xff;
		hd.dlh = (flen >> 8) & 0xff;
		if (opt.isBasic) {
			hd.type = 0x00;
			hd.asl = opt.aStart & 0xff;
			hd.ash = (opt.aStart >> 8) & 0xff;
			hd.pll = hd.dll;
			hd.plh = hd.dlh;
		} else {
			hd.type = 0x03;
			hd.asl = 0;
			hd.ash = 0;
			hd.pll = 0;
			hd.plh = 0x80;
		}
		tapAddBlock(file, sizeof(tHead), (char*)&hd, 0x00);
	}
	tapAddBlock(file, flen, buf, 0xff);		// data
	fclose(file);
}

void tapPop(char* img, char* fname, char* oname) {
	printf("Not developed yet\n");
}

void tapRename(char* ipath, char* oname, char* nname) {
	printf("Not developed yet\n");
}

// scl

void sclCreate(char* path) {
	char data[] = {'S', 'I', 'N', 'C', 'L', 'A', 'I', 'R', 0x00, 0x55, 0x02, 0x00, 0x00};	// last 4 is CRC
	savefile(path, data, 13);
}

void sclList(char* path) {
	FILE* file = fopen(path, "rb");
	if (!file) {
		printf("Can't open image '%s'\n",path);
		return;
	}
	trdFile hd;
	printf("Name\t\tExt\tStart\tSize\tSLen\n---------------------------\n");
	fseek(file, 8, SEEK_SET);
	int cnt = fgetc(file);
	unsigned short start, len;
	while (cnt > 0) {
		fread((char*)&hd, 14, 1, file);
		start = hd.stl | (hd.sth << 8);
		len = hd.lnl | (hd.lnh << 8);
		printf("%.8s\t%c\t%i\t%i\t%i\n",hd.name, hd.ext, start, len, hd.slen);
		cnt--;
	}
	fclose (file);
}

void sclPush(char* ipath, char* fpath) {
	char buf[0x10000];
	trdFile hd = prepareFile(fpath, buf);
	if (hd.name[0] == 0) return;
	FILE* file = fopen(ipath, "rb");
	if (!file) {
		printf("Can't open image '%s'\n", ipath);
		return;
	}
	char bufH[9];			// head
	fread(bufH, 9, 1, file);
	int cnt = bufH[8];
	int clen = cnt * 14;
	char bufC[clen];		// catalog
	fread(bufC, clen, 1, file);
	int dsiz = 0;
	for (int i = 0; i < cnt; i++) {
		dsiz += (unsigned)bufC[14 * i + 13];
	}
	dsiz <<= 8;
	char bufD[dsiz];		// data
	fread(bufD, dsiz, 1, file);
	fclose(file);
	if ((unsigned)bufH[8] > 127) {
		printf("Too much files in image\n");
		return;
	}
	bufH[8]++;
//	trdFile hd = makeDSC(fpath, ilen);
	file = fopen(ipath, "wb");
	if (!file) {
		printf("Can't open image '%s' for writing\n",ipath);
		return;
	}
	fwrite(bufH, 9, 1, file);		// header
	fwrite(bufC, clen, 1, file);		// old catalog
	fwrite((char*)&hd, 14, 1, file);	// new DSC
	fwrite(bufD, dsiz, 1, file);		// old data
	fwrite(buf, hd.slen << 8, 1, file);	// new data
	int crc = getSum(bufH, 9);
	crc += getSum(bufC, clen);
	crc += getSum((char*)&hd, 14);
	crc += getSum(bufD, dsiz);
	crc += getSum(buf, hd.slen << 8);
	fputi(file, crc);
	fclose(file);
}

void sclPop(char* ipath, char* fname, char* oname) {
	FILE* file = fopen(ipath, "rb");
	if (!file) {
		printf("Can't open file '%s'\n",ipath);
		return;
	}
	char nbuf[9];
	char buf[0x10000];
	makeName(fname, nbuf);
	// printf("searching name '%.9s'\n",nbuf);
	fseek(file, 8, SEEK_SET);
	trdFile hd;
	int cnt = fgetc(file);
	int pos = 9 + cnt * 14;	// data pos
	int find = 0;
	while ((cnt > 0) && (find == 0)) {
		fread((char*)&hd, 14, 1, file);
		if (memcmp(hd.name, nbuf, 9) == 0) {
			fseek(file, pos, SEEK_SET);
			fread(buf, hd.slen << 8, 1, file);
			find = 1;
		} else {
			pos += (hd.slen << 8);
			cnt--;
		}
	}
	fclose(file);
	if (!find) {
		printf("File '%s' not found in image\n",fname);
	} else {
		saveoutput(oname, buf, hd);
	}
}

int sclSeekFile(FILE* file, char* name, trdFile* hd) {
	fseek(file, 8, SEEK_SET);
	int cnt = fgetc(file);
	int find = 0;
	int pos = 9;
	while ((cnt > 0) && !find) {
		fread((char*)hd, 14, 1, file);
		if (memcmp((char*)hd, name, 9) == 0) {
			find = 1;
		} else {
			pos += 14;
			cnt--;
		}
	}
	return find ? pos : -1;
}

void sclRename(char* ipath, char* oname, char* nname) {
	FILE* file = fopen(ipath, "r+");
	if (!file) {
		printf("Can't open file '%s'\n",ipath);
		return;
	}
	char nbuf[9];
	makeName(oname, nbuf);
	trdFile hd;
	int pos = sclSeekFile(file, nbuf, &hd);
	if (pos < 0) {
		printf("File '%s' not found in image\n",oname);
	} else {
		int oldSum = getSum(nbuf, 9);
		makeName(nname, nbuf);
		int dif = getSum(nbuf, 9) - oldSum;	// diff to new CRC
		fseek(file, pos, SEEK_SET);		// read old CRC
		fwrite(nbuf, 9, 1, file);
		fseek(file, -4, SEEK_END);
		int crc = fgeti(file);
		crc += dif;				// change it
		fseek(file, -4, SEEK_END);		// write back
		fputi(file, crc);
	}
	fclose(file);
}

// trd

void trdCreate(char* path) {
	char buf[0xa0000];
	memset(buf, 0x00, 0xa0000);
	buf[0x8e2] = 0x01;
	buf[0x8e3] = 0x16;
	buf[0x8e5] = 0xf0;
	buf[0x8e6] = 0x09;
	buf[0x8e7] = 0x10;
	FILE *file = fopen(path, "wb");
	if (!file) {
		printf("Can't write to file '%s'\n",path);
	} else {
		fwrite(buf, 0xa0000, 1, file);
		fclose(file);
	}
}

void trdList(char* path) {
	FILE* file = fopen(path, "rb");
	if (!file) {
		printf("Can't open image '%s'\n", path);
		return;
	}
	printf("Name\t\tExt\tStart\tSize\tSLen\tSec\tTrk\n---------------------------\n");
	trdFile hd;
	int cnt = 0;
	int start,len;
	while (cnt < 128) {
		fread((char*)&hd, 16, 1, file);
		if (hd.name[0] == 0x00) break;
		if (hd.name[0] != 0x01) {
			start = hd.stl | (hd.sth << 8);
			len = hd.lnl | (hd.lnh << 8);
			printf("%.8s\t%c\t%i\t%i\t%i\t%i\t%i\n", hd.name, hd.ext, start, len, hd.slen, hd.sec, hd.trk);
		}
	}
	fclose(file);
}

void trdPush(char* ipath, char* fpath) {
	char buf[0x10000];
	trdFile hd = prepareFile(fpath, buf);
	if (hd.name[0] == 0) return;
	char img[0xa0000];
	if (readfile(ipath, img, 0xa0000) < 0) {
		printf("Can't read image '%s'\n", ipath);
		return;
	}
//	trdFile hd = makeDSC(fpath, ilen);
	
	int files = img[0x8e4];			// test files count
	if (files > 127) {
		printf("Too many files in image\n");
		return;
	}
	int freesec = (img[0x8e5] & 0xff) | ((img[0x8e6] & 0xff) << 8);
	
	if (freesec < hd.slen) {		// test free sectors
		printf("No room for file\n");
		return;
	}
	
	int lastsec = img[0x8e1] & 0xff;
	int lasttrk = img[0x8e2] & 0xff;
	hd.sec = lastsec;
	hd.trk = lasttrk;
	files++;
	img[0x8e4] = files;
	freesec -= hd.slen;
	img[0x8e5] = freesec & 0xff;
	img[0x8e6] = ((freesec & 0xff00) >> 8);
	char* ptr = img;
	while (*ptr)				// find 1st free descriptor
		ptr += 16;
	memcpy(ptr, (char*)&hd, 16);		// make new descriptor
	int secnum = ((hd.trk << 4) + hd.sec);	// free sector num
	int fpos = (secnum << 8); 		// position in image
	secnum += hd.slen;
	img[0x8e1] = secnum & 0x0f;
	img[0x8e2] = ((secnum & 0xfff0) >> 4);
	memcpy(img + fpos, buf, hd.slen << 8);	// copy file data
	savefile(ipath, img, 0xa0000);
}

int trdSeekFile(FILE* file, char* name, trdFile* hd) {
	int find = 0;
	int cnt = 0;
	while ((cnt < 128) && !find) {
		fread((char*)hd, 16, 1, file);
		if (hd->name[0] == 0x00) break;
		if (memcmp((char*)hd, name, 9) == 0) {
			find = 1;
		}
		cnt++;
	}
	return find ? (cnt - 1) << 4 : -1;
}

void trdPop(char* ipath, char* fname, char* oname) {
	FILE* file = fopen(ipath, "rb");
	if (!file) {
		printf("Can't open image '%s'\n",ipath);
		return;
	}
	char nbuf[9];
	char buf[0x10000];
	makeName(fname, nbuf);
	trdFile hd;
	if (trdSeekFile(file, nbuf, &hd) < 0) {
		printf("Can't find file '%s' in image\n",fname);
	} else {
		fseek(file, ((hd.trk << 4) | (hd.sec & 0x0f)) << 8, SEEK_SET);
		fread(buf, hd.slen << 8, 1, file);
		saveoutput(oname, buf, hd);
	}
	fclose(file);
}

void trdRename(char* ipath, char* oname, char* nname) {
	FILE* file = fopen(ipath, "r+");
	if (!file) {
		printf("Can't open image '%s'\n",ipath);
		return;
	}
	char nbuf[9];
	makeName(oname, nbuf);
	trdFile hd;
	int pos = trdSeekFile(file, nbuf, &hd);
	if (pos < 0) {
		printf("Can't find file '%s' in image\n",oname);
	} else {
		makeName(nname, nbuf);
		fseek(file, pos, SEEK_SET);
		fwrite(nbuf, 9, 1, file);
	}
	fclose(file);
}

// commands

void create(char* path) {
	if (testExt(path, "tap")) {
		tapCreate(path);
	} else if (testExt(path, "scl")) {
		sclCreate(path);
	} else if (testExt(path, "trd")) {
		trdCreate(path);
	} else {
		printf("Unknown type, use TAP SCL TRD please\n");
	}
}

void list(char* path) {
	int mode = getFileType(path);
	switch (mode) {
		case TYPE_TAP:
			tapList(path);
			break;
		case TYPE_SCL:
			sclList(path);
			break;
		case TYPE_TRD:
			trdList(path);
			break;
		default:
			printf("Unknown image type\n");
			break;
	}
}

void push(char* ipath, char* fpath) {
	int type = getFileType(ipath);
	switch(type) {
		case TYPE_TAP:
			tapPush(ipath, fpath);
			break;
		case TYPE_SCL:
			sclPush(ipath, fpath);
			break;
		case TYPE_TRD:
			trdPush(ipath, fpath);
			break;
		default:
			printf("Unknown image type\n");
			break;
	}
}

void pop(char* ipath, char* fname, char* oname) {
	if (!oname) {
		if (opt.asHobeta) {
			char fpath[1024];
			memset(fpath, 0, 1024);
			char* pDot = strrchr(fname,'.');
			if (pDot) {
				memcpy(fpath, fname, pDot - fname + 1);
				strcat(fpath,"$");
				strcat(fpath,pDot + 1);
				oname = fpath;
			} else {
				oname = fname;
			}
		} else {
			oname = fname;
		}
	}
	int type = getFileType(ipath);
	switch(type) {
		case TYPE_TAP:
			tapPop(ipath, fname, oname);
			break;
		case TYPE_SCL:
			sclPop(ipath, fname, oname);
			break;
		case TYPE_TRD:
			trdPop(ipath, fname, oname);
			break;
		default:
			printf("Unknown image type\n");
			break;
	}
}

void renam(char* ipath, char* oname, char* nname) {
	int type = getFileType(ipath);
	switch(type) {
		case TYPE_TAP:
			tapRename(ipath, oname, nname);
			break;
		case TYPE_SCL:
			sclRename(ipath, oname, nname);
			break;
		case TYPE_TRD:
			trdRename(ipath, oname, nname);
			break;
		default:
			printf("Unknown image type\n");
			break;
	}
}

void help() {
	printf("::: Usage :::\n");
	printf("mctrd [-h][-z][-b][-n][-a num] command name1 [name2] [name3]\n");
	printf("::: Keys :::\n");
	printf("%*s %s\n",-32,"-a | --autostart NUM","set autostart line number for basic file");
	printf("%*s %s\n",-32,"-b | --basic","add file to archive as basic");
	printf("%*s %s\n",-32,"-h | --help","show this page");
	printf("%*s %s\n",-32,"-n | --no-head","push file to TAP without header");
	printf("%*s %s\n",-32,"-z | --hobeta","pop/push file as hobeta (SCL, TRD only)");
	printf("::: Commands :::\n");
	printf("%*s %s\n",-32,"list <image>","show image catalog");
	printf("%*s %s\n",-32,"new <image>","create new image (TAP, TRD, SCL: depends on extension)");
	printf("%*s %s\n",-32,"add <file> <image>","put file into image");
	printf("%*s %s\n",-32,"pop <file> <image> [<outname>]","extract file from image");
	printf("%*s %s\n",-32,"rename <image> <oname> <nname>","rename file inside image. oname = old name, nname = new name");
}

int main(int ac,char* av[]) {
	char* com = NULL;
	char* fname1 = NULL;
	char* fname2 = NULL;
	char* fname3 = NULL;
	opt.isBasic = 0;
	opt.headLess = 0;
	opt.asHobeta = 0;
	opt.mono = 0;
	opt.aStart = 0;
	int i = 1;
	char* parg;
	while (i < ac) {
		parg = av[i++];
		if ((strcmp(parg,"-a") == 0) || (strcmp(parg,"--autostart") == 0)) {
			if (i < ac) {
				opt.aStart = atoi(av[i++]);
			} else {
				printf("Invalid argument count\n");
				return 1;
			}
		} else if ((strcmp(parg,"-b") && strcmp(parg,"--basic")) == 0) {
			opt.isBasic = 1;
		} else if ((strcmp(parg,"-h") && strcmp(parg,"--help")) == 0) {
			help();
			return 0;
		} else if ((strcmp(parg,"-n") && strcmp(parg,"--no-head")) == 0) {
			opt.headLess = 1;
		} else if ((strcmp(parg,"-z") && strcmp(parg,"--hobeta")) == 0) {
			opt.asHobeta = 1;
		} else if ((strcmp(parg,"-m") && strcmp(parg,"--mono")) == 0) {
			opt.mono = 1;
		} else {
			if (!com) com = parg;
			else if (!fname1) fname1 = parg;
			else if (!fname2) fname2 = parg;
			else if (!fname3) fname3 = parg;
		}
	}
	if (!fname1) help();
	else if (strcmp(com,"list") == 0) list(fname1);
	else if (strcmp(com,"new") == 0) create(fname1);
	else if (!fname2) help();
	else if (strcmp(com,"pop") == 0) pop(fname2, fname1, fname3);
	else if (strcmp(com,"add") == 0) push(fname2, fname1);
	else if (!fname3) help();
	else if (strcmp(com,"rename") == 0) renam(fname1, fname2, fname3);
	else help();
	return 0;
}
