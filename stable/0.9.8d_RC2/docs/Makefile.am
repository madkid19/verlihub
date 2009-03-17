# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs

EXTRA_DIST = ascii features.html using.txt help params.php verlihub.html configuring.txt help.html plugins_devel.html WIN_INSTALL.txt help.sql html_manual pdf_manual

dist-hook:
	rm -rf `find $(distdir)/html_manual -name CVS`
	rm -rf `find  $(distdir)/pdf_manual -name CVS`

