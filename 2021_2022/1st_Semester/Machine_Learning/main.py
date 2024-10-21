import numpy as np
import cv2
from matplotlib import pyplot as plt

faceCasc = cv2.CascadeClassifier('cascade/cascade.xml')
nums = list(range(352,624))

for i in nums:
    name = "./positive/000"+ str(i) + ".jpg"
    img = cv2.imread(name)
    gray=cv2.cvtColor(img,cv2.COLOR_RGB2GRAY)
    faces=faceCasc.detectMultiScale(gray,1.3,6)

    for(q,w,e,r) in faces:  
        img=cv2.rectangle(img,(q,w),(q+e, w+r),(255,0,0),2)
        
    p,l,m = cv2.split(img)

    img = cv2.merge([m,l,p])
    plt.imshow(img)
    plt.show()
    cv2.waitKey(30)
    cv2.destroyAllWindows()