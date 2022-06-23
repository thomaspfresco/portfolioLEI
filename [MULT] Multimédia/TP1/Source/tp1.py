# -*- coding: utf-8 -*-

"""
Created on Sat Feb 19 17:00:59 2022

@author: 
Filipe Ribeiro - 2019223576,
Miguel Ferreira - 2019214567, 
Thomas Fresco - 2019219057
"""

from distutils import ccompiler
from tty import CC
import matplotlib.pyplot as plt
import matplotlib.colors as clr
import numpy as np
from scipy.fftpack import dct, idct
#import cv2
#import math as m

################################################################################################

#dimensões da imagem original
r_original = 0
c_original = 0

ds_opt = 0 #4:2:2 (422) ou 4:2:0 (420)
dct_opt = 0 # imagem toda (0), blocos de 8x8 (8), blocos de 64x64 (64)
quality = 0 # para quantizacao

R = 0 #canal red na imagem original
G = 0 #canal green na imagem original
B = 0 #canal blue na imagem original

################################################################################################

#le a imagem e envia para outra funcao
def ler_imagem():
    #name_img = input("Image name: ")
    #name_img = name_img.split(" ")
    img = plt.imread('barn_mountains' + '.bmp') #name_img[0]

    global r_original,c_original,ds_opt,dct_opt,quality
    r_original, c_original, ch_original = img.shape

    ds_opt = 420 #int(name_img[1])
    dct_opt = 8 #int(name_img[2])
    quality = 100

    plot1("Imagem Original",img,0)

    return img

#funcao para plot individual
def plot1(title,img,cm):
    plt.figure()
    if cm == 0:
        plt.imshow(img)
    else:
        plt.imshow(img,cm)
    plt.title(title)
    plt.axis('off')
    plt.show()

#funcao para plot triplo
def plot3(stitle,title1,img1,cm1,title2,img2,cm2,title3,img3,cm3):
    fig = plt.figure(figsize=(10, 5))
    plt.suptitle(stitle)

    fig.add_subplot(1, 3, 1)
    plt.imshow(img1,cm1)
    plt.title(title1)
    plt.axis('image')

    fig.add_subplot(1, 3, 2)
    plt.imshow(img2,cm2)
    plt.title(title2)
    plt.axis('image')

    fig.add_subplot(1, 3, 3)
    plt.imshow(img3,cm3)
    plt.title(title3)
    plt.axis('image')
    plt.show() 

################################################################################################
################################################################################################

#ENCODER =======================================================================================
def encoder(img):

    global dct_opt
    
    #separar os canais RGB
    splitRGB(img)
    
    #padding
    img_padded = padding(img)
    #conversão YCbCr
    img_ycbcr = RGB_YCbCr(img_padded)
    #downsampling
    Y_d, Cb_d, Cr_d = downsampling(img_ycbcr)
    #dct
    Y_dct, Cb_dct, Cr_dct = dctImg(Y_d, Cb_d, Cr_d)

    if dct_opt == 8:
        #quantizacao
        Y_Q, Cb_Q, Cr_Q = quantization(Y_dct, Cb_dct, Cr_dct)
        #dcpm
        Y_dcpm, Cb_dcpm, Cr_dcpm = dcpm(Y_Q, Cb_Q, Cr_Q)
        
        return  Y_dcpm, Cb_dcpm, Cr_dcpm
    else:
        return Y_dct, Cb_dct, Cr_dct

#DECODER =====================================================================================
def decoder(Y, Cb, Cr):

    global dct_opt

    #juncao dos canais RGB
    joinRGB()

    if dct_opt == 8:
        #dcpm
        Y_Q, Cb_Q, Cr_Q = dcpm_inv(Y, Cb, Cr)
        #inv quantization
        Y_dct,Cb_dct,Cr_dct = quantization_inv(Y_Q, Cb_Q, Cr_Q)
        #inv dct
        Y_d, Cb_d, Cr_d = dct_inv(Y_dct,Cb_dct,Cr_dct)
    else:
        #inv dct
        Y_d, Cb_d, Cr_d = dct_inv(Y,Cb,Cr)
    #upsampling
    img_upsampled = upsampling(Y_d, Cb_d, Cr_d)
    #decode YCbCr
    img_padded = YCbCr_RGB(img_upsampled)
    #unpad
    img_original = unpad(img_padded)

    return img_original

################################################################################################
################################################################################################

def splitRGB(img):
    
    global R,G,B

    R = img[:, :, 0]
    G = img[:, :, 1]
    B = img[:, :, 2]
    r, c = R.shape

    cmRed = clr.LinearSegmentedColormap.from_list("myred",[(0,0,0), (1,0,0)], N = 256)
    cmGreen = clr.LinearSegmentedColormap.from_list("mygreen",[(0,0,0), (0,1,0)], N = 256)
    cmBlue = clr.LinearSegmentedColormap.from_list("myblue",[(0,0,0), (0,0,1)], N = 256)
    
    plot3("RGB","Red",R,cmRed,"Green",G,cmGreen,"Blue",B,cmBlue) 

#faz o padding da imagem de forma a ficar com uma resolucao multipla de 16x16
def padding(img):

    global r_original,c_original,dct_opt

    #ajuste do padding para blocos 64x64
    if dct_opt == 64:
        factor = 128
    else:
        factor = 16

    #padding vertical
    rows = r_original%factor

    if rows != 0:
        missing_lines = factor - rows + 1
        aux_lines = np.ones(r_original, dtype=int)
        aux_lines[r_original-1] = missing_lines 
        img = np.repeat(img, aux_lines, axis=0)       

    #padding horizontal
    columns = c_original%factor

    if columns != 0:
        missing_columns = factor - columns + 1
        aux_columns = np.ones(c_original, dtype=int)
        aux_columns[c_original-1] = missing_columns
        img = np.repeat(img, aux_columns, axis=1)

    plot1("Imagem com Padding",img,0)

    return img

#converte o ficheiro para YCbCr
def RGB_YCbCr(img):
    
    img_convertida = np.dot(img, Tc.T)
    img_convertida[:, :, 1] += 128
    img_convertida[:, :, 2] += 128

    plot3("YCbCr","Y",img_convertida[:, :, 0],cmGray,"Cb",img_convertida[:, :, 1],cmGray,"Cr",img_convertida[:, :, 2],cmGray)  
    
    return img_convertida

def downsampling(imgConv):

    global ds_opt

    #sepração em canais
    Y_d = imgConv[:, :, 0]
    Cb = imgConv[:, :, 1]
    Cr = imgConv[:, :, 2]

    #4:2:2 -> Cb e Cr, x
    if ds_opt == 422:
        title = "Downsampling 4:2:2"
        Cb_d = Cb[:, ::2]
        Cr_d = Cr[:, ::2]
    #4:2:0 -> Cb e Cr, x e y
    else:
        title = "Downsampling 4:2:0"
        Cb_d = Cb[::2, :]
        Cb_d = Cb_d[:, ::2]
        Cr_d = Cr[::2, :]
        Cr_d = Cr_d[:, ::2]

    plot3(title,"Y_d",Y_d,cmGray,"Cb_d",Cb_d,cmGray,"Cr_d",Cr_d,cmGray) 

    return Y_d, Cb_d, Cr_d
 
# compressão por Transformada de Coseno Discreta (DCT)
def dctImg(Y_d,Cb_d,Cr_d):

    global dct_opt

    Yl,Yc = Y_d.shape
    Cl,Cc = Cb_d.shape

    if dct_opt == 8 or dct_opt == 64 or dct_opt == 4 or dct_opt == 2:

        for i in range(0, Yl, dct_opt):
            for j in range(0, Yc, dct_opt):
                Y_d[i:i + dct_opt, j:j + dct_opt] = dct(dct(Y_d[i:i + dct_opt, j:j + dct_opt], norm="ortho").T, norm="ortho").T
                    
        for i in range(0, Cl, dct_opt):
            for j in range(0, Cc, dct_opt):
                Cb_d[i:i + dct_opt, j:j + dct_opt] =dct(dct(Cb_d[i:i + dct_opt, j:j + dct_opt], norm="ortho").T, norm="ortho").T
                Cr_d[i:i + dct_opt, j:j + dct_opt] = dct(dct(Cr_d[i:i + dct_opt, j:j + dct_opt], norm="ortho").T, norm="ortho").T

        if dct_opt == 8:
            plot3("DCT 8x8","Y DCT",np.log(np.abs(Y_d) + 0.0001),cmGray,"Cb DCT",np.log(np.abs(Cb_d) + 0.0001)
            ,cmGray,"Cr DCT",np.log(np.abs(Cr_d) + 0.0001),cmGray)  
        elif dct_opt == 64:
            plot3("DCT 64x64","Y DCT",np.log(np.abs(Y_d) + 0.0001),cmGray,"Cb DCT",np.log(np.abs(Cb_d) + 0.0001)
            ,cmGray,"Cr DCT",np.log(np.abs(Cr_d) + 0.0001),cmGray)  
        elif dct_opt == 4:
            plot3("DCT 4x4","Y DCT",np.log(np.abs(Y_d) + 0.0001),cmGray,"Cb DCT",np.log(np.abs(Cb_d) + 0.0001)
            ,cmGray,"Cr DCT",np.log(np.abs(Cr_d) + 0.0001),cmGray)    
        elif dct_opt == 2:
            plot3("DCT 2x2","Y DCT",np.log(np.abs(Y_d) + 0.0001),cmGray,"Cb DCT",np.log(np.abs(Cb_d) + 0.0001)
            ,cmGray,"Cr DCT",np.log(np.abs(Cr_d) + 0.0001),cmGray)  

    else: 

        Y_d = dct(dct(Y_d, norm="ortho").T, norm="ortho").T
        Cb_d = dct(dct(Cb_d, norm="ortho").T, norm="ortho").T
        Cr_d = dct(dct(Cr_d, norm="ortho").T, norm="ortho").T

        plot3("DCT","Y DCT",np.log(np.abs(Y_d) + 0.0001),cmGray,"Cb DCT",np.log(np.abs(Cb_d) + 0.0001),cmGray,
        "Cr DCT",np.log(np.abs(Cr_d) + 0.0001),cmGray)  

    return Y_d, Cb_d, Cr_d

def quantization(Y_dct, Cb_dct, Cr_dct):

    global dct_opt,quality

    Yl,Yc = Y_dct.shape
    Cl,Cc = Cb_dct.shape

    #apenas arredondamento
    if quality == 100:
        Y_dct = np.round(Y_dct)
        Cb_dct = np.round(Cb_dct)
        Cr_dct = np.round(Cr_dct)

    else:
        for i in range(0, Yl, dct_opt):
            for j in range(0, Yc, dct_opt):
                Y_dct[i:i + dct_opt, j:j + dct_opt] = np.round(Y_dct[i:i + dct_opt, j:j + dct_opt]/Q_matriz_Y)
                        
        for i in range(0, Cl, dct_opt):
            for j in range(0, Cc, dct_opt):
                Cb_dct[i:i + dct_opt, j:j + dct_opt] = np.round(Cb_dct[i:i + dct_opt, j:j + dct_opt]/Q_matriz_C)
                Cr_dct[i:i + dct_opt, j:j + dct_opt] = np.round(Cr_dct[i:i + dct_opt, j:j + dct_opt]/Q_matriz_C)

    plot3("Quantization","Y DCT",np.log(np.abs(Y_dct) + 0.0001),cmGray,"Cb DCT",np.log(np.abs(Cb_dct) + 0.0001)
            ,cmGray,"Cr DCT",np.log(np.abs(Cr_dct) + 0.0001),cmGray)

    #imgDCT_Q = imgDCT.copy().astype(np.int16)
    return Y_dct, Cb_dct, Cr_dct

def dcpm(Y_Q,Cb_Q,Cr_Q):

    global dct_opt

    Yl, Yc = Y_Q.shape
    Cl, Cc = Cb_Q.shape

    dcY0 = 0
    dcCb0 = 0
    dcCr0 = 0

    for i in range(0, Yl, dct_opt):
        for j in range(0, Yc, dct_opt):
            if i == 0 and j == 0:
                dcY0 = Y_Q[i, j]
                continue
            dc = Y_Q[i, j]
            diff = dc - dcY0
            Y_Q[i, j] = diff
            dcY0 = dc
    
    for i in range(0, Cl, dct_opt):
        for j in range(0, Cc, dct_opt):
            if i == 0 and j == 0:
                dcCb0 = Cb_Q[i, j]
                dcCr0 = Cr_Q[i, j]
                continue
            dcb = Cb_Q[i, j]
            dcr = Cr_Q[i, j]
            diffb = dcb - dcCb0
            diffr = dcr - dcCr0
            Cb_Q[i, j] = diffb
            Cr_Q[i, j] = diffr
            dcCb0 = dcb
            dcCr0 = dcr
    
    plot3("DCMP","Y DCPM",np.log(np.abs(Y_Q) + 0.0001),cmGray,"Cb DCPM",np.log(np.abs(Cb_Q) + 0.0001)
            ,cmGray,"Cr DCPM",np.log(np.abs(Cr_Q) + 0.0001),cmGray)
        
    return Y_Q, Cb_Q, Cr_Q
    
#########################################################################################################
#########################################################################################################

def joinRGB():
    global R,G,B,r_original,c_original

    final = np.zeros((r_original, c_original, 3), dtype=np.uint8) 
    final[:, :, 0] = R
    final[:, :, 1] = G
    final[:, :, 2] = B
    
    #plot1("Imagem Reconstruida",final,0)

def dcpm_inv(Y_dcpm, Cb_dcpm, Cr_dcpm):

    global dct_opt
    
    Yl, Yc = Y_dcpm.shape
    Cl, Cc = Cb_dcpm.shape

    dcY0 = 0
    dcCb0 = 0
    dcCr0 = 0

    for i in range(0, Yl, dct_opt):
        for j in range(0, Yc, dct_opt):
            if i == 0 and j == 0:
                dcY0 = Y_dcpm[i, j]
                continue
            diff = Y_dcpm[i, j]
            dc = dcY0 + diff
            Y_dcpm[i, j] = dc
            dcY0 = dc

    for i in range(0, Cl, dct_opt):
        for j in range(0, Cc, dct_opt):
            if i == 0 and j == 0:
                dcCb0 = Cb_dcpm[i, j]
                dcCr0 = Cr_dcpm[i, j]
                continue
            diffCb = Cb_dcpm[i, j]
            diffCr = Cr_dcpm[i, j]
            dcCb = dcCb0 + diffCb
            dcCr = dcCr0 + diffCr
            Cb_dcpm[i, j] = dcCb
            Cr_dcpm[i, j] = dcCr
            dcCb0 = dcCb
            dcCr0 = dcCr

    return Y_dcpm, Cb_dcpm, Cr_dcpm

def quantization_inv(Y_Q, Cb_Q, Cr_Q):

    global dct_opt,quality

    Yl,Yc = Y_Q.shape
    Cl,Cc = Cb_Q.shape

    if quality != 100:

        for i in range(0, Yl, dct_opt):
            for j in range(0, Yc, dct_opt):
                Y_Q[i:i + dct_opt, j:j + dct_opt] = np.round(Y_Q[i:i + dct_opt, j:j + dct_opt]*Q_matriz_Y)
                        
        for i in range(0, Cl, dct_opt):
            for j in range(0, Cc, dct_opt):
                Cb_Q[i:i + dct_opt, j:j + dct_opt] = np.round(Cb_Q[i:i + dct_opt, j:j + dct_opt]*Q_matriz_C)
                Cr_Q[i:i + dct_opt, j:j + dct_opt] = np.round(Cr_Q[i:i + dct_opt, j:j + dct_opt]*Q_matriz_C)
    
    #plot3("teste q inv","Y",np.log(np.abs(Y_Q) + 0.0001),cmGray,"Cb",np.log(np.abs(Cb_Q) + 0.0001)
    #        ,cmGray,"Cr",np.log(np.abs(Cr_Q) + 0.0001),cmGray)

    return Y_Q,Cb_Q,Cr_Q

def dct_inv(Y_dct,Cb_dct,Cr_dct):
    
    global dct_opt

    Yl,Yc = Y_dct.shape
    Cl,Cc = Cb_dct.shape

    if dct_opt == 8 or dct_opt == 64 or dct_opt == 4 or dct_opt == 2:

        for i in range(0, Yl, dct_opt):
            for j in range(0, Yc, dct_opt):
                Y_dct[i:i+dct_opt, j:j+dct_opt] = idct(idct(Y_dct[i:i + dct_opt,j:j + dct_opt], norm="ortho").T, norm="ortho").T

        for i in range(0, Cl, dct_opt):
            for j in range(0, Cc, dct_opt):
                Cb_dct[i:i + dct_opt, j:j + dct_opt] = idct(idct(Cb_dct[i:i + dct_opt, j:j + dct_opt], norm="ortho").T, norm="ortho").T
                Cr_dct[i:i + dct_opt, j:j + dct_opt] = idct(idct(Cr_dct[i:i + dct_opt, j:j + dct_opt], norm="ortho").T, norm="ortho").T
    
    else:
    
        Y_dct = idct(idct(Y_dct, norm="ortho").T, norm="ortho").T
        Cb_dct = idct(idct(Cb_dct, norm="ortho").T, norm="ortho").T
        Cr_dct = idct(idct(Cr_dct, norm="ortho").T, norm="ortho").T

    return Y_dct, Cb_dct, Cr_dct

def upsampling(Y_d, Cb_d, Cr_d):

    global ds_opt
    
    #4:2:2 -> Cb e Cr, x
    if ds_opt == 422:
        Cb = np.repeat(Cb_d, 2, axis=1)
        Cr = np.repeat(Cr_d, 2, axis=1)
    #4:2:0 -> Cb e Cr, x e y
    else:
        Cb = np.repeat(Cb_d, 2, axis=1)
        Cb = np.repeat(Cb, 2, axis=0)
        Cr = np.repeat(Cr_d, 2, axis=1)
        Cr = np.repeat(Cr, 2, axis=0)

    '''final = np.zeros((l, c, 3)) 
    final[:, :, 0] = Y_d v
    final[:, :, 1] = Cb
    final[:, :, 2] = Cr'''

    final = np.dstack((Y_d,Cb,Cr)) #juncao dos 3 canais

    return final

#conversao YCbCr -> RGB
def YCbCr_RGB(img):

    TcInvertida = np.linalg.inv(Tc)

    img_final = img
    img_final[:, :, 1] -= 128
    img_final[:, :, 2] -= 128
    img_final = np.dot(img, TcInvertida.T)  

    img_final = np.round(img_final) #arredondamento
    img_final = np.where(img_final > 255, 255, img_final) #fix overflow (>255)
    img_final = np.where(img_final < 0, 0, img_final) #fix overflow (<0)
    img_final = img_final.astype(np.uint8) #converter para uint8

    #plot1("YCbCr -> RGB",img_final,0)

    return img_final 

#remove o padding
def unpad(padded):
    
    global r_original, c_original
    unpadded = padded[:r_original, :c_original, :] 
    plot1("Imagem Reconstruida",unpadded,0)

    return unpadded

#########################################################################################################
#########################################################################################################

if __name__ == "__main__":
    
    #plt.close('all')

    cmGray = clr.LinearSegmentedColormap.from_list("myGray",[(0,0,0), (1,1,1)], N = 256)

    Tc = np.array([[0.299, 0.587, 0.114],
            [-0.168736, -0.331264, 0.5],
            [0.5, -0.418688, -0.081312]])

    img = ler_imagem()

    #matriz de quantizacao Y
    Q_matriz_Y = np.array(
    [[16, 11, 10, 16, 24, 40, 51, 61],
    [12, 12, 14, 19, 26, 58, 60, 55],
    [14, 13, 16, 24, 40, 57, 69, 56], 
    [14, 17, 22, 29, 51, 87, 80, 62], 
    [18, 22, 37, 56, 68, 109, 103, 77],
    [24, 35, 55, 64, 81, 104, 113, 92],
    [49, 64, 78, 87, 103, 121, 120, 101],
    [72, 92, 95, 98, 112, 100, 103, 99]]) 

    #matriz de quantizacao Cb e Cr
    Q_matriz_C = np.array(
    [[17, 18, 24, 47, 99, 99, 99, 99],
    [18, 21, 26, 66, 99, 99, 99, 99],
    [24, 26, 56, 99, 99, 99, 99, 99],
    [47, 66, 99, 99, 99, 99, 99, 99],
    [99, 99, 99, 99, 99, 99, 99, 99],
    [99, 99, 99, 99, 99, 99, 99, 99],
    [99, 99, 99, 99, 99, 99, 99, 99],
    [99, 99, 99, 99, 99, 99, 99, 99]])

    #alteracao das matrizes de quamtizacao consoante a qualidade
    if quality != 0:
        Q_matriz_Y = np.round((Q_matriz_Y * ((100 - quality) / 50)).astype('uint8'))
        Q_matriz_Y[Q_matriz_Y > 255] = 255
        Q_matriz_Y = Q_matriz_Y.astype(np.uint8)
        Q_matriz_C = np.round((Q_matriz_C * ((100 -  quality) / 50)).astype('uint8'))
        Q_matriz_C[Q_matriz_C > 255] = 255
        Q_matriz_C = Q_matriz_C.astype(np.uint8)

    Y_Q, Cb_Q, Cr_Q = encoder(img)
    final = decoder(Y_Q, Cb_Q, Cr_Q)
    #print(final)