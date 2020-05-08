WebPrintServer

Webpage printing tool, call desktop printer to print webpage data, support image，html

Api

	GET /version
	Get the version of the current print service
	
	GET /getAllPrinter
	Get all printers on the current desktop
	
	GET /getDefaultPrinter
	Get the default printer of the current desktop
	
	POST /printNow?printer=cannon&page=width*height&margin=left*top*right*bottom&type=img/html/txt&code=raw/base64/url
	Directly call the printer to print, the print data is passed in through POST
	printer	Optional	Specify the printer
	page	Optional	Paper size,width*height
	margin	Optional	Paper margin,left*top*right*bottom
	type	Optional	Data printing method, img/html/txt, default:html
	code	Optional	Encoding of POST data，raw/base64/url, default:raw
	
	POST /printPreview?printer=cannon&page=width*height&margin=left*top*right*bottom&type=img/html/txt&code=raw/base64/url
	Printing preview, the print data is passed in through POST
	printer	Optional	Specify the printer
	page	Optional	Paper size,width*height
	margin	Optional	Paper margin,left*top*right*bottom
	type	Optional	Data printing method, img/html/txt, default:html
	code	Optional	Encoding of POST data，raw/base64/url, default:raw
