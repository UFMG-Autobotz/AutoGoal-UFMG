#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cv2
import numpy as np
from math import sqrt

class Vision_Functions():
	def __init__(self):
		self.set_functions_dict()

	def set_functions_dict(self):
		self.func_dict = {}
		self.func_dict['Escala de cinza'] = lambda frame: cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		self.func_dict['Detectar BOLA'] = lambda frame: self.color_detect(frame, ['laranja'])
		self.func_dict['Detectar time amarelo'] = lambda frame: self.robot_detect(frame, ['amarelo'])
		self.func_dict['Detectar time azul'] = lambda frame: self.robot_detect(frame, ['azul'])

	def get_functions_nicks(self):
		return self.func_dict.keys()

	def run(self, img, nick):
		return self.func_dict[nick](img)

	def color_detect(self, im, color_list):
		hsv = cv2.cvtColor(im, cv2.COLOR_BGR2HSV)
		colors = { 'vermelho1' : range(11),
					'laranja' : range(11,21),
					'amarelo' : range(21,46),
					'verde' : range(46,81),
					'ciano' : range(81,105),
					'azul' : range(105,128),
					'roxo' : range(128,146),
					'rosa' : range(146,170),
					'vermelho2' : range(170,180)}
		if len(color_list) == 1:
			color = color_list[0]
			lower_threshold = np.array([colors[color][0], 50, 50])
			upper_threshold = np.array([colors[color][-1], 255, 255])
			mask = cv2.inRange(hsv, lower_threshold, upper_threshold)
			# Mais filtros para máscara
			element = cv2.getStructuringElement(cv2.MORPH_RECT,(3,3))
		    	mask = cv2.erode(mask,element, iterations=2)
		    	mask = cv2.dilate(mask,element,iterations=2)
		    	mask = cv2.erode(mask,element)
			# Cria contornos
			_, contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
		    	maximumArea = 0
		    	bestContour = None
		    	for contour in contours:
				currentArea = cv2.contourArea(contour)
				if currentArea > maximumArea:
			    		bestContour = contour
			    		maximumArea = currentArea
		     	# Cria quadrados
		    	if bestContour is not None:
				x,y,w,h = cv2.boundingRect(bestContour)
				cv2.rectangle(im, (x,y),(x+w,y+h), (0,0,255), 1)

			return im
		else:
			print("Ai que delicia")
			'''
			im_list = []
			for color in color_list:
				lower_threshold = np.array([colors[color][0], 50, 50])
				upper_threshold = np.array([colors[color][-1], 255, 255])
				mask = cv2.inRange(hsv, lower_threshold, upper_threshold)
				# Mais filtros para máscara
				element = cv2.getStructuringElement(cv2.MORPH_RECT,(3,3))
		    		mask = cv2.erode(mask,element, iterations=2)
		    		mask = cv2.dilate(mask,element,iterations=2)
		    		mask = cv2.erode(mask,element)
				# Cria contornos
				_, contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
			    	maximumArea = 0
			    	bestContour = None
			    	for contour in contours:
					currentArea = cv2.contourArea(contour)
					if currentArea > maximumArea:
				    		bestContour = contour
				    		maximumArea = currentArea
				# Cria quadrados
				if bestContour is not None:
					x,y,w,h = cv2.boundingRect(bestContour)
					cv2.rectangle(im_i, (x,y),(x+w,y+h), (0,0,255), 1)
				im_i = cv2.bitwise_and(im,im, mask= mask)
				im_list.append(im_i)
			res = im_list[0]
			for im_i in im_list[1:]:
				res = cv2.add(res,im_i)
			return res
			'''

	def robot_detect(self,im,color_list):
		hsv = cv2.cvtColor(im, cv2.COLOR_BGR2HSV)
		colors = {	'amarelo' : range(21,46),
					'azul' : range(105,128)}
		color = color_list[0]
		lower_threshold = np.array([colors[color][0], 50, 50])
		upper_threshold = np.array([colors[color][-1], 255, 255])
		mask = cv2.inRange(hsv, lower_threshold, upper_threshold) #Criar imagem binária
		mask = self.noise_reduction(mask) #Limpar ruídos
		_, contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE) #Detectar contornos
		noField = self.no_field_mask(hsv)
		_, contours2, hierarchy = cv2.findContours(noField, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE) #Detectar contornos
		i = 0
		while (i < 3) and (i < len(contours)):
			(x,y),radius = cv2.minEnclosingCircle(contours[i])
			center = (int(x),int(y))
			radius = int(radius)
			im = cv2.circle(im,center,radius,(0,0,255),2)

			(X,Y),radius = cv2.minEnclosingCircle(contours2[0])
			currentDist = sqrt((x-X)**2) + ((y-Y)**2)
			j = 1
			while j < len(contours2):
				(X2,Y2),radius = cv2.minEnclosingCircle(contours2[j])
				dist = sqrt((x-X2)**2) + ((y-Y2)**2)
				if (dist < currentDist):
					currentDist = dist
					X = X2
					Y = Y2
				j = j+1
			centerSecond = (int(X),int(Y))
			centerRobot = (int((X+x)/2),int((Y+y)/2))
			cv2.arrowedLine(im, center, centerSecond, upper_threshold, 3)
			im = cv2.circle(im,centerRobot,5,(250,0,0),1)
			i = i + 1

		# cv2.rectangle(im, (x,y),(x+w,y+h), (0,0,255), 1)
		return im

	def noise_reduction(self,im):
		element = cv2.getStructuringElement(cv2.MORPH_RECT,(3,3))
		im = cv2.erode(im,element, iterations=2)
		im = cv2.dilate(im,element,iterations=2)
		im = cv2.erode(im,element)
		return im

	def no_field_mask(self,im):
		#amarelo
		lower_threshold = (21, 50, 50)
		upper_threshold = (46, 255, 255)
		maskAmarelo = cv2.inRange(im, lower_threshold, upper_threshold) #Criar imagem binária
		#azul
		lower_threshold = (105, 50, 50)
		upper_threshold = (128, 255, 255)
		maskAzul = cv2.inRange(im, lower_threshold, upper_threshold) #Criar imagem binária
		#laranja
		lower_threshold = (11, 50, 50)
		upper_threshold = (21, 255, 255)
		maskLaranja = cv2.inRange(im, lower_threshold, upper_threshold) #Criar imagem binária
		#campo
		lower_threshold = (0, 0, 0)
		upper_threshold = (180, 50, 50)
		mask = cv2.inRange(im, lower_threshold, upper_threshold) #Criar imagem binária
		mask = cv2.bitwise_not(mask)
		mask = mask - (maskAzul + maskAmarelo + maskLaranja)
		mask = self.noise_reduction(mask)
		return mask

if __name__ == '__main__':
	pass
