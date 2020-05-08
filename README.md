WebPrintServer
网页打印工具

使用方法
	GET /version
	获取当前打印服务的版本
	GET /getAllPrinter
	获取当前桌面所有打印机
	GET /getDefaultPrinter
	获取当前桌面的默认打印机
	POST /printNow?printer=cannon&page=width*height&margin=left*top*right*bottom&type=img/html/txt&code=raw/base64/url
	直接调用打印机打印，打印数据通过POST传入
	printer	可选	指定打印机
	page	可选	纸张大小,格式width*height
	margin	可选	纸张边距,格式left*top*right*bottom
	type	可选	数据打印方式，选项img/html/txt，默认html
	code	可选	POST数据的编码，选项raw/base64/url, 默认raw
	POST /printPreview?printer=cannon&page=width*height&margin=left*top*right*bottom&type=img/html/txt&code=raw/base64/url
	printer	可选	指定打印机
	page	可选	纸张大小,格式width*height
	margin	可选	纸张边距,格式left*top*right*bottom
	type	可选	数据打印方式，选项img/html/txt，默认html
	code	可选	POST数据的编码，选项raw/base64/url, 默认raw