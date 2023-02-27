import csv
import openpyxl
import pathlib
#*** insert your text file path here ***
text_file='logs\sample-20022023.txt'
#***************************************
header=['Total Fuel Pulses','Total Water Pulses','Fuel Pulse Period (s)','Fuel Flowrate (mL/min)','Cycle Position (/5)','Water Percentage (%)','Fuel Percentage (%)']
text_file.replace('\\','/')
parent_dir=pathlib.Path(text_file).parent
xlsx_file=parent_dir.joinpath('sample-20022023.xlsx')
wb=openpyxl.Workbook()
ws=wb.worksheets[0]
with open(text_file, 'r') as txt_file:
    text_file=filter(lambda txt: len(txt)==7 or len(txt)==8 or txt.find(',') != -1, txt_file)
    reader=csv.reader(text_file,delimiter=',')
    ws.append(header)
    for row in reader:
        row=[float(i) for i in row] if str(row).find(',') != -1 else row
        ws.append(row)
wb.save(xlsx_file)