#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cv2
import numpy as np

class Vision_Functions():
	def __init__(self):
		self.set_functions_dict()

	def set_functions_dict(self):
		self.func_dict = {}
		self.func_dict['Escala de cinza'] = lambda frame: cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		self.func_dict['Detectar azul'] = lambda frame: self.color_detect(frame, ['azul'])
		self.func_dict['Detectar verde'] = lambda frame: self.color_detect(frame, ['verde'])
		self.func_dict['Detectar laranja'] = lambda frame: self.color_detect(frame, ['laranja'])
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
		#Encontrar maiores áreas:
		'''maximumArea = 0
		bestContour = None
		for contour in contours:
		currentArea = cv2.contourArea(contour)
		if currentArea > maximumArea:
				bestContour = contour
				maximumArea = currentArea'''
		# Cria quadrados
		# if bestContour is not None:
		# x,y,w,h = cv2.boundingRect(bestContour)
		(x,y),radius = cv2.minEnclosingCircle(contours[0])
		center = (int(x),int(y))
		radius = int(radius)
		im = cv2.circle(im,center,radius,(0,0,255),2)

		# cv2.rectangle(im, (x,y),(x+w,y+h), (0,0,255), 1)
		return im

	def noise_reduction(self,im):
		element = cv2.getStructuringElement(cv2.MORPH_RECT,(3,3))
		im = cv2.erode(im,element, iterations=2)
		im = cv2.dilate(im,element,iterations=2)
		im = cv2.erode(im,element)
		return im

if __name__ == '__main__':
	pass
