import pygame
from pygame.locals import QUIT, KEYDOWN, KEYUP, K_ESCAPE, K_1, K_2, K_3, K_4, K_SPACE
from time import perf_counter

def play_sound(pygame, button_state, num_sound_buttons):
    # construct mp3 string
    str = ''
    channel = 0
    for i in range(num_sound_buttons):
        if button_state[i]:
            str = '1' + str  # probably bad time
            channel += 2 ** i
        else:
            str = '0' + str

    print("str: ", str, "channel: ", channel)
    file_name = str + '.mp3'
    try:
        pygame.mixer.Channel(channel).play(pygame.mixer.Sound('./audio/' + file_name))
    except:
        print("exception occurred playing sound")


def stop_all(pygame, num_sound_buttons):
    for i in range(2 ** num_sound_buttons):
        pygame.mixer.Channel(i).stop()


def main():
    num_sound_buttons = 4

    delay_ms = 50  # delay starts from first button press

    pygame.init()
    pygame.mixer.set_num_channels(2 ** 4)
    screen = pygame.display.set_mode([500, 500])

    pygame.mixer.init()

    print("Press any key to play the sound. Press 'Esc' to exit.")

    first_press_time = perf_counter()
    chord_started = False

    sound_button_state = [False] * num_sound_buttons
    muted = False

    while True:
        for event in pygame.event.get():

            if event.type == QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
                print("Exiting")
                pygame.quit()
                return
            elif event.type == KEYDOWN:

                if event.key == K_1:
                    sound_button_state[0] = True
                elif event.key == K_2:
                    sound_button_state[1] = True
                elif event.key == K_3:
                    sound_button_state[2] = True
                elif event.key == K_4:
                    sound_button_state[3] = True
                elif event.key == K_SPACE:
                    # stop all sounds
                    print("Muting, Stopping all sounds")
                    stop_all(pygame, num_sound_buttons)
                    chord_started = False
                    sound_button_state = [False] * num_sound_buttons
                    muted = True
                    continue
                else:
                    continue

                if not chord_started:
                    first_press_time = perf_counter()
                    chord_started = True

            elif event.type == KEYUP:
                if event.key == K_SPACE:
                    print("Unmuting")
                    muted = False
                elif event.key == K_1:
                    sound_button_state[0] = False
                elif event.key == K_2:
                    sound_button_state[1] = False
                elif event.key == K_3:
                    sound_button_state[2] = False
                elif event.key == K_4:
                    sound_button_state[3] = False

        if chord_started and (perf_counter() - first_press_time) * 1000 > delay_ms:
            if not muted:
                # play sound
                play_sound(pygame, sound_button_state, num_sound_buttons)

            chord_started = False


if __name__ == "__main__":
    main()
