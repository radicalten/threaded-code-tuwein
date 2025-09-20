#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

// Standard Wii Homebrew boilerplate
static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

// Macro definitions from your original code
#define NEXT break
#define guard(n) asm volatile("#" #n)

// Your original benchmark logic, moved into its own function
void run_benchmark() {
    static int prog[] = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5};
    int *ip = prog;
    int count = 100000000;

    for (;;) {
        switch (*ip++) {
            case 0:
                guard(0);
                NEXT;
            case 1:
                guard(1);
                NEXT;
            case 2:
                guard(2);
                NEXT;
            case 3:
                guard(3);
                NEXT;
            case 4:
                guard(4);
                NEXT;
            case 5:
                if (count > 0) {
                    count--;
                    ip = prog;
                    NEXT;
                } else {
                    return;
                }
            /* The rest is to get gcc to make a realistic switch statement */
            case 12:
            case 8:
                count = 25;
                NEXT;
            case 10:
                count--;
                NEXT;
        }
    }
}

// Main function for the Wii application
int main(int argc, char **argv) {
    // 1. Initialize hardware subsystems
    VIDEO_Init();
    WPAD_Init();
    
    // 2. Configure the video mode
    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    // ------------------- THE FIX IS HERE -------------------
    // Initialize the GX graphics subsystem. This MUST be done
    // before calling console_init(), which uses GX to draw text.
    GX_Init(xfb, rmode);
    // -------------------------------------------------------

    // Setup a black background
    GXColor background = {0, 0, 0, 0xff};
    GX_SetCopyClear(background, 0x00ffffff);

    // 3. Initialize the console for printf output
    console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
    
    // 4. Finalize video setup and make it visible
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    // 5. Run the application logic
    printf("\n\nHello Wii! Preparing to run benchmark...\n");
    printf("Press the HOME button to exit at any time.\n\n");
    sleep(2);
    printf("Starting benchmark (100,000,000 iterations)...\n");

    run_benchmark();
    
    printf("Benchmark finished!\n");
    printf("The application will now loop. Press HOME to exit.\n");

    // 6. Main loop to keep the application running
    while (1) {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);

        if (pressed & WPAD_BUTTON_HOME) {
            break; // Exit the loop to end the program
        }

        VIDEO_WaitVSync();
    }

    return 0;
}
