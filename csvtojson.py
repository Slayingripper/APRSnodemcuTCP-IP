# import csv
# import json

# csvfile = open('SUOMI_VIIRS_C2_Global_24h.csv', 'r')
# jsonfile = open('viirs.json', 'w')




# fieldnames = ("latitude","longitude","bright_ti4","scan","track","acq_date",
#               "acq_time","satellite","condidence","version","bright","frp","daynight")
# reader = csv.DictReader( csvfile, fieldnames)
# for row in reader:
#     json.dump(row, jsonfile)
#     jsonfile.write('\n')

    
import pandas as pd
csv_file = pd.DataFrame(pd.read_csv("SUOMI_VIIRS_C2_Global_24h.csv", sep = ",", header = 0, 
                                    index_col = False,usecols= ['latitude', 'longitude']))

# filteredcsv = csv_file[ (34.511083 > csv_file.latitude > 35.844535) &
#                         (34.661865 > csv_file.longitude > 31.816406)]
filteredcsv = csv_file[ (csv_file.latitude <= 35.844535) & (csv_file.latitude >= 34.511083) &
                        (csv_file.longitude >= 31.816406) & (csv_file.longitude <= 34.661865)]
print(filteredcsv)
filteredcsv.to_json("viirs.json",
                  orient = "records", date_format = "epoch", double_precision = 10, 
                  force_ascii = True, date_unit = "ms", default_handler = None)    