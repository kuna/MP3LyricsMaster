class APICSearcher {
private:
	char imgSrc[1024];
public:
	bool searchAPIC(WCHAR *artist);
	bool getAPIC(WCHAR *artist, WCHAR *out);
private:
	bool getBetweenTag(char *org, char *sTag, char *eTag, char *out);
};