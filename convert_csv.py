import csv
with open('logs/sample-20022023.txt', 'r') as txt_file:
    output=filter(lambda txt: txt.find('M') != -1 or txt.find(',') != -1, txt_file)
    output=(line.strip() for line in output)
    print(output)
    with open('logs/sample-20022023.csv', 'w') as csv_file:
        header=['Total Fuel Pulses','Total Water Pulses','Fuel Pulse Period (s)','Fuel Flowrate (mL/min)','Cycle Position (/5)','Water Percentage (%)','Fuel Percentage (%)']
        csv_reader=csv.reader(output, delimiter=',')
        csv_reader=(line.strip() for line in csv_reader)
        csv_writer=csv.writer(csv_file, delimiter=',')
        csv_writer.writerow(header)
        csv_writer.writerows(csv_reader)
    # for line in txt_file:
    #     if line.find('M') != -1 or line.find(',') != -1:
    #         in_txt = csv.reader(line, delimiter = ',')
    #         print(in_txt)
            # out_csv = csv.writer(csv_file)
            # out_csv.(in_txt)