    
import pandas as pd
import wget
import os

url = "https://firms.modaps.eosdis.nasa.gov/data/active_fire/suomi-npp-viirs-c2/csv/SUOMI_VIIRS_C2_Global_24h.csv"

filePath = 'SUOMI_VIIRS_C2_Global_24h.csv';
# As file at filePath is deleted now, so we should check if file exists or not not before deleting them
if os.path.exists(filePath):
    os.remove(filePath)

wget.download(url, 'SUOMI_VIIRS_C2_Global_24h.csv')
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