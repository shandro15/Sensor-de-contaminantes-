function doGet(e) {

Logger.log( JSON.stringify(e));

var result = 'Ok';

if (e.parameter == 'undefined') {

result = 'No Parameters';

}

else {

var sheet_id = '1KxuU_FU8l-MREgMtDDpos-WozJYUQ4w-pAKfVbMYnsU'; // Spreadsheet ID

var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet();

var newRow = sheet.getLastRow() + 1;

var rowData = [];

var Curr_Date = new Date();

rowData[0] = Curr_Date; // Date in column A

var Curr_Time = Utilities.formatDate(Curr_Date, "Asia/Jakarta", 'HH:mm:ss');

rowData[1] = Curr_Time; // Time in column B

for (var param in e.parameter) {

Logger.log('In for loop, param=' + param);

var value = stripQuotes(e.parameter[param]);

Logger.log(param + ':' + e.parameter[param]);

switch (param) {

case 'nitrogen':

rowData[2] = value; // Nitrogen in column C

result += ' ,Nitrogen Written on column C';

break;

case 'monoxide':

rowData[3] = value; // Monoxide in column D

result += ' ,Monoxide Written on column D';

break;

case 'ozone':

rowData[4] = value; // Ozone in column E

result += ' ,Ozone Written on column E';

break;

case 'dioxide':

rowData[5] = value; // Dioxide in column F

result += ' ,Dioxide Written on column F';

break;

case 'temperature':

rowData[6] = value; // Temperature in column G

result = 'Temperature Written on column G';

break;

case 'humidity':

rowData[7] = value; // Humidity in column H

result += ' ,Humidity Written on column H';

break;

default:

result = "unsupported parameter";

}

}

Logger.log(JSON.stringify(rowData));

var newRange = sheet.getRange(newRow, 1, 1, rowData.length);

newRange.setValues([rowData]);

}

return ContentService.createTextOutput(result);

}

function stripQuotes( value ) {

return value.replace(/^["']|['"]$/g, "");

}
