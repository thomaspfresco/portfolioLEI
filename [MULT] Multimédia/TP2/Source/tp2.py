#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr  6 13:03:06 2021

@author: rpp
"""

import librosa #https://librosa.org/
import librosa.display
import librosa.beat
import sounddevice as sd  #https://anaconda.org/conda-forge/python-sounddevice
import warnings
import numpy as np
import matplotlib.pyplot as plt
import os
import scipy.stats as st
from scipy.spatial import distance

#2.1.1
def read_file(fileName):
    #fileName = './features/top100_features.csv'
    top100 = np.genfromtxt(fileName, delimiter=',')
    nl, nc = top100.shape
    #print("dim ficheiro top100_features.csv original = ", nl, "x", nc)
    #print()
    #print(top100)
    
    top100 = top100[1:, 1:(nc-1)]
    nl, nc = top100.shape
    #print()
    #print("dim top100 data = ", nl, "x", nc)
    #print()
    #print(top100)

    return top100

#2.1.2
def normalize(top100):
    top100_N = np.zeros(top100.shape)
    nl, nc = top100_N.shape
    
    for i in range(nc):
        vmax = top100[:, i].max()
        vmin = top100[:, i].min()
        top100_N[:, i] = (top100[:, i] - vmin)/(vmax - vmin)

    #print(top100_N)

    return top100_N

#2.1.3
def save_matrix(matrix, fileName):
    np.savetxt(fileName, matrix, fmt = "%lf", delimiter=',')
    
    #checando
    matrix = np.genfromtxt(fileName, delimiter=',')
    nl, nc = matrix.shape
    #print("dim ficheiro top100_features_normalized_data.csv = ", nl, "x", nc)
    #print()
    #print(top100_N)

#2.2.1 e 2.2.2
def extract_features():
    warnings.filterwarnings("ignore")

    filesPath = './dataset/all/'
    files = os.listdir(filesPath)
    files.sort()
    numFiles = len(files)

    allSongs = np.zeros((numFiles, 190))

    # Extract features of each song
    for i in range(numFiles):
        features = []

        # Song to extract
        inFile = librosa.load(filesPath + files[i], sr=22050, mono=True)[0]

        # Extract mfcc
        mfcc = librosa.feature.mfcc(inFile, n_mfcc=13)
        features.append(mfcc)

        #print("\n mfcc size = ", mfcc.shape, "\n")
        #print("\n mfcc coeficients: \n", mfcc)


        # Extract spectral centroid
        spectral_centroid = librosa.feature.spectral_centroid(inFile)
        features.append(spectral_centroid)

        # Extract spectral bandwith
        spectral_bandwidth = librosa.feature.spectral_bandwidth(inFile)
        features.append(spectral_bandwidth)

        # Extract spectral contrast
        spectral_contrast = librosa.feature.spectral_contrast(inFile, n_bands=6)
        features.append(spectral_contrast)

        # Extract spectral flatness
        spectral_flatness = librosa.feature.spectral_flatness(inFile)
        features.append(spectral_flatness)

        # Extract spectral rollof
        spectral_rolloff = librosa.feature.spectral_rolloff(inFile)
        features.append(spectral_rolloff)

        # Extract F0
        F0 = librosa.yin(inFile, fmin=20, fmax=11025)
        F0[F0 == 11025] = 0
        features.append(F0)

        # Extract rms
        rms = librosa.feature.rms(inFile)
        features.append(rms)

        # Extract zero_crossing_rate
        zero_crossing_rate = librosa.feature.zero_crossing_rate(inFile)
        features.append(zero_crossing_rate)

        # Extract tempo
        tempo = librosa.beat.tempo(inFile)


        #calculo das estatisticas
        allFeatures = np.array([])
        for feature in features:
            try:
                r, _ = feature.shape
            except:
                r = feature.shape[0]
                feature = feature.reshape((1, r), order='F')
                r, _ = feature.shape

            addFeature = np.zeros((r, 7))
            for j in range(r):
                mean = np.mean(feature[j, :])
                stdDev = np.std(feature[j, :])
                skew = st.skew(feature[j, :])
                kurtosis = st.kurtosis(feature[j, :])
                median = np.median(feature[j, :])
                maxv = np.max(feature[j, :])
                minv = np.min(feature[j, :])

                addFeature[j, :] = np.array([mean, stdDev, skew, kurtosis, median, maxv, minv])

            addFeature = addFeature.flatten()
            allFeatures = np.append(allFeatures, addFeature)

        allFeatures = np.append(allFeatures, tempo)
        allSongs[i] = allFeatures

    return allSongs

#3.1
def metricas_similaridade_librosa():
    features = np.genfromtxt('./features/extracted_features.csv', delimiter=',')
    
    #substitui os valores nan por 0
    for i in range(900):
        for j in range(190):
            if (np.isnan(features[i][j])):
                features[i][j] = 0


    euclidiana = np.zeros((900, 900))
    manhattan = np.zeros((900, 900))
    cosseno = np.zeros((900, 900))
    for i in range(900):
        for j in range(i+1, 900):
            euclidiana[i][j] = distance.euclidean(features[i, :], features[j, :])
            euclidiana[j][i] = euclidiana[i][j]

            manhattan[i][j] = distance.cityblock(features[i, :], features[j, :])
            manhattan[j][i] = manhattan[i][j]

            cosseno[i][j] = distance.cosine(features[i, :], features[j, :])
            cosseno[j][i] =  cosseno[i][j]

    save_matrix(euclidiana, './features/euclidiana_librosa.csv')
    save_matrix(manhattan, './features/manhattan_librosa.csv')
    save_matrix(cosseno, './features/cosseno_librosa.csv')

#3.2
def metricas_similaridade_top100():
    features = np.genfromtxt('./features/top100_features_normalized_data.csv', delimiter=',')

    euclidiana = np.zeros((900, 900))
    manhattan = np.zeros((900, 900))
    cosseno = np.zeros((900, 900))
    for i in range(900):
        for j in range(i+1, 900):
            euclidiana[i][j] = distance.euclidean(features[i, :], features[j, :])
            euclidiana[j][i] = euclidiana[i][j]

            manhattan[i][j] = distance.cityblock(features[i, :], features[j, :])
            manhattan[j][i] = manhattan[i][j]

            cosseno[i][j] = distance.cosine(features[i, :], features[j, :])
            cosseno[j][i] =  cosseno[i][j]

    save_matrix(euclidiana, './features/euclidiana_top100.csv')
    save_matrix(manhattan, './features/manhattan_top100.csv')
    save_matrix(cosseno, './features/cosseno_top100.csv')

#3.3
def similaridade_queries():
    names = ['./features/euclidiana_librosa.csv', './features/manhattan_librosa.csv', './features/cosseno_librosa.csv', './features/euclidiana_top100.csv', './features/manhattan_top100.csv', './features/cosseno_top100.csv']
    query_names = ['MT0000202045.mp3', 'MT0000379144.mp3', 'MT0000414517.mp3', 'MT0000956340.mp3']

    #index das musicas da query
    songs = os.listdir('./dataset/all')
    query_index = []
    for i in range(4):
        query_index.append(songs.index(query_names[i]))
    
    ret_array = []
    for i in range(6):
        for j in range(4):
            indexes = ranking(names[i], query_index[j])
            print("Indices do top 20 do ficheiro " + names[i] + " para a query " + query_names[j] + ":")
            print(indexes[-20:])
            ret_array.append(indexes[-20:])
            print("Correspondem as musicas: ")
            song_names = []
            for k in range(1, 21):
                song_names.append(songs[indexes[k]])
            print(song_names)
            print("\n")

    return ret_array
            
            

#aux 3.3
def ranking(filename, query_index):
    features = np.genfromtxt(filename, delimiter=",")

    top20 = np.empty(21)
    top20 = np.argsort(features[query_index][:])
    top20 = top20[:21]
    #print(top20)

    return top20


#4.1.1
def ranking_metadados(index):
    metadataRawMatrix = np.genfromtxt('dataset/panda_dataset_taffc_metadata.csv', delimiter=',', dtype="str")
    metadata = metadataRawMatrix[1:, [1, 3, 9, 11]]

    metadataScores = np.zeros((1, 900))
    for i in range(metadata.shape[0]):
        score = 0
        for j in range(metadata.shape[1]):
            #teste para artista e quadrante:
            if j < 2:
                if metadata[index, j] == metadata[i, j]:
                    score = score + 1
            else:
                #teste para MoodStrSplit e GenresStr
                listA = metadata[index, j][1:-1].split('; ')#retira as "" do começo e fim e separa no "; "
                listB = metadata[i, j][1:-1].split('; ')
                for e in listA:
                    for ee in listB:
                        if e == ee:
                            score = score + 1
        metadataScores[0, i] = score

    metadataScores[0, index] = -1
    #print(metadataScores)
    top20metadata = np.empty(20)
    top20metadata = np.flip(np.sort(metadataScores[0]))
    #print(top20metadata[:20])

    top20 = np.empty(20)
    top20 = np.flip(np.argsort(metadataScores[0]))
    top20 = top20[:20]
    
    return top20

###################################################################################


if __name__ == "__main__":
    plt.close('all')

    #2
    '''top100 = read_file('./features/top100_features.csv')
    top100_N = normalize(top100)
    save_matrix(top100_N, './features/top100_features_normalized_data.csv')'''

    '''allSongs = extract_features()
    allSongs_N = normalize(allSongs)
    save_matrix(allSongs_N, './features/extracted_features.csv')'''

    #metricas_similaridade_librosa()
    #metricas_similaridade_top100()
    array_query = similaridade_queries()

    #indices das musicas da query -> 10 26 28 59
    array_metadados = []
    array_metadados.append(ranking_metadados(10))
    array_metadados.append(ranking_metadados(26))
    array_metadados.append(ranking_metadados(28))
    array_metadados.append(ranking_metadados(59))

    names = ['./features/euclidiana_librosa.csv', './features/manhattan_librosa.csv', './features/cosseno_librosa.csv', './features/euclidiana_top100.csv', './features/manhattan_top100.csv', './features/cosseno_top100.csv']
    query_names = ['MT0000202045.mp3', 'MT0000379144.mp3', 'MT0000414517.mp3', 'MT0000956340.mp3']
    for i in range(6):
        for j in range(4):
            precision = np.intersect1d(array_query[i], array_metadados[j]).size / 20
            print("Precision para a query " + query_names[j] + " para o ficheiro " + names[i] + ": " + str(precision))
    
    songs = os.listdir('./dataset/all')
    for i in range(4):
        musicas = []
        for j in range(20):
            musicas.append(songs[array_metadados[i][j]])

        print("Top 20 de musicas para a query " + query_names[i]  + ": ")
        print(musicas)
    
    #recall =  #relevant retrievied / total relevant dataset



    