import xml.etree.ElementTree as ET
import sys

def convertName(inputFileUri, outputFileUri) :
    with open(inputFileUri, 'r') as f :
        xmlData = ET.parse(f)
        root = xmlData.getroot()

    for child in root:
        if(child.tag == 'data') :
            name = child.attrib['name']
            name = name.replace('.', '_')
            
            if(name.find('placeholder') > 0) :
                name = name + '.PlaceholderText'
            else :
                name = name + '.Text'
            
            child.attrib['name'] = name

    with open(outputFileUri, 'wb') as f :
        xmlData.write(f, encoding='utf-8', xml_declaration=True)

def main():
    fileUri = sys.argv[1]
    outputUri = sys.argv[2]
    
    convertName(fileUri, outputUri)
    
if __name__ == '__main__':
    main()
