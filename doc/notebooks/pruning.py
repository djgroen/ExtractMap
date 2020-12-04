# Copyright Know-Center GmbH, 2020  
# Meizhu Wang, Christoph Schweimer, and Bernhard Geiger (cschweimer@know-center.at; bgeiger@know-center.at; geiger@ieee.org)

import pandas as pd
import numpy as np
from geopy.distance import geodesic
from itertools import combinations 

def geo_pruning(x,df,k,b):
    distances=[]
    x1=df[df['#name']==x.name]['lat'].values
    y1=df[df['#name']==x.name]['lon'].values
    for xx in x.index:
        x2=df.iloc[xx]['lat']
        y2=df.iloc[xx]['lon']
        distance=geodesic((x1,y1),(x2,y2)).meters
        distances.append(distance)
    knn=sorted(distances)[k]
    distances=[d if d<=knn*b else 0 for d in distances]
    return distances

def knnroute(x,k):
    knn=x.nsmallest(k).iloc[-1]
    return x.where(x<=knn)

def rSubset(arr, r): 
    return list(combinations(arr, r))

def trianglepruning(x,triangle_factor, DEBUGGING=False):
    x2=pd.DataFrame(columns=x.columns, index=x.index)
    for num,column in enumerate(list(x.columns)[2:]):
        print('Location '+str(num)+' out of '+str(len(x.columns)))
        knns=list(x[column][x[column].notna()].index)
        knn_distances=list(x[column][x[column].notna()])
        dictionary = dict(zip(knns, knn_distances))
        knn_combinations=rSubset(knns, 2)
        for comb in knn_combinations:
            c=x.loc[comb[0],comb[1]]
            a=dictionary[comb[0]]
            b=dictionary[comb[1]]
            triangle=[a,b,c]
            longest=triangle.pop(triangle.index(max(triangle)))
            if longest>triangle_factor*(sum(triangle)):
                if longest==a:
                    x2.loc[comb[0],column]=1
                    if DEBUGGING:
                        print(str(comb[0])+' - '+column+' via '+str(comb[1]))
                elif longest==b:
                    x2.loc[comb[1],column]=1
                    if DEBUGGING:
                        print(str(comb[1])+' - '+column+' via '+str(comb[0]))
                elif longest==c:
                    x2.loc[comb[0],comb[1]]=1
                    if DEBUGGING:
                        print(str(comb[0])+' - '+str(comb[1])+' via '+column)
    return x.where(x2.isna().astype(bool))

def save_to_csv(x,filename):
    res=x[x!=0].stack().reset_index()
    res=res.rename({'Index':'name1','name':'name2',0:'distance'},axis=1)
    res.distance=res.distance/1000
    res.to_csv(filename,index=False)

    
def pruning_function(Distances,BETA_LIST,REGION,ALGORITHM='triangle',KNN_LIST=None,ALPHA_LIST=None):
    if ALGORITHM=='triangle':
        for triangle_factor in BETA_LIST:
            Distances_Local=Distances.copy()
            Distances_Local=Distances_Local.replace(0,np.NaN)
            Distances_Local['Index']=list(Distances.columns)
            Distances_Local.set_index('Index',inplace=True)
            Distances_Local = Distances_Local.where(np.triu(np.ones(Distances_Local.shape)).astype(np.bool))
            Distances_Local=trianglepruning(Distances_Local,triangle_factor);
            save_to_csv(Distances_Local,REGION+'/Distances_%.3f.csv' % triangle_factor)
    elif ALGORITHM=='knn':
        for k in KNN_LIST:
            Distances_knn=Distances.replace(0,np.NaN)
            Distances_knn['Index']=list(Distances.columns)
            Distances_knn.set_index('Index',inplace=True)
            Distances_knn=Distances_knn.apply(knnroute,args=(k,))
            save_to_csv(Distances_knn,REGION+'/Distances_%d.csv' % k)
    elif ALGORITHM=='geo':
        for k in KNN_LIST:
            for alpha in ALPHA_LIST:
                Distances_geo=Distances.apply(geo_pruning,args=(Locations,k,alpha,))
                Distances_geo=Distances.where(Distances_geo>0)
                Distances_geo=Distances_geo.replace(to_replace=40000000,value=np.nan)
                Distances_geo['Index']=list(Distances.columns)
                Distances_geo.set_index('Index',inplace=True)
                save_to_csv(Distances_geo,path+'Distances_%d_%.3f.csv' % (k, triangle_factor))
    elif ALGORITHM=='geo_triangle':
        for k in KNN_LIST:
            for alpha in ALPHA_LIST:
                Distances_geo=Distances.apply(geo_pruning,args=(Locations,k,alpha,))
                for triangle_factor in BETA_LIST:
                    # The following line makes the dataframe symmetric
                    Distances_geo_triangle=pd.DataFrame(np.maximum(Distances_geo.values,Distances_geo.values.T), \
                                           index=Distances_geo.index,columns=Distances_geo.columns)
                    Distances_geo_triangle=Distances_geo_triangle.replace(0,np.NaN)
                    Distances_geo_triangle['Index']=list(Distances_geo.columns)
                    Distances_geo_triangle.set_index('Index',inplace=True)
                    Distances_geo_triangle = Distances_geo_triangle.where(np.triu(np.ones(Distances_geo.shape)).astype(np.bool))
                    Distances_geo_triangle=trianglepruning(Distances_geo_triangle,triangle_factor);
                    save_to_csv(Distances_geo_triangle,path+'Distances_%d_%.3f_%.3f.csv' % (k, alpha, triangle_factor))
    else:
        print('Other Algorithms are currently not supported.')