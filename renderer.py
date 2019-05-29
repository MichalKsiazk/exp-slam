import pygame
import sys
import time
import math

APS = 0.176643768

class Renderer(object):

    def __init__(self, W, H):
        self.color = (0, 200, 0)
        self.size = width , height = W , H
        self.screen = pygame.display.set_mode(self.size)
        self.zero_point = (W * 0.5, H * 0.5)
        self.draw_zero_point()
        self.flip()
        print("renderer init")

    def draw_point(self, dst, steps, scale):
        global ANGLE_PER_STEP
        x = dst * math.sin(math.radians(steps * APS)) * scale
        y =  dst * math.cos(math.radians(steps * APS)) * scale
        print(x, y)
        self.draw_pixel(x, y, self.color)
        self.flip()

    def flip(self):
        pygame.display.flip()

    def clear(self):
        self.screen.fill((0,0,0))
        draw_zero_point()
        pygame.display.flip()

    def set_color(self, r, g, b):
        self.color = (r, g, b)
    
    def draw_zero_point(self):
        for x in range(-5, 6):
            self.draw_pixel(x, 0, (250,0,0))
        for y in range(-5, 6):
            self.draw_pixel(0, y, (250,0,0))

    def draw_pixel(self, x, y, color):
        self.screen.set_at((int(x + self.zero_point[0]), int(y + self.zero_point[1])), color)
