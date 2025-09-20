#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Needed for memset
#include <unistd.h> // Needed for sleep()
#include <gccore.h>
#include <wiiuse/wpad.h>

// Standard Wii Homebrew boilerplate
static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

// GX command buffer (FIFO)
#define DEFAULT_FIFO_SIZE (256 * 1024)
static void *gp_fifo = NULL;

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
    // 1. Initialize hardware
    VIDEO_Init();
    WPAD_Init();
    
    // 2. Configure video mode and get a framebuffer
    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    // 3. Initialize the console for printf output
    console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
    
    // 4. Allocate and initialize the Graphics Processor (GX) command buffer (FIFO)
    gp_fifo = mem_align(32, DEFAULT_FIFO_SIZE);
    memset(gp_fifo, 0, DEFAULT_FIFO_SIZE);
    GX_Init(gp_fifo, DEFAULT_FIFO_SIZE);

    // 5. Setup a black background
    GXColor background = {0, 0, 0, 0xff};
    GX_SetCopyClear(background, 0x00ffffff);
    
    // 6. Finalize video setup and make the framebuffer visible
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    // 7. Run the application logic
    printf("\n\nHello Wii! Preparing to run benchmark...\n");
    printf("Press the HOME button to exit at any time.\n\n");
    
    sleep(2); // This will now work
    
    printf("Starting benchmark (100,000,000 iterations)...\n");

    run_benchmark();
    
    printf("Benchmark finished!\n");
    printf("The application will now loop. Press HOME to exit.\n");

    // 8. Main loop to keep the application running
    while (1) {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);

        if (pressed & WPAD_BUTTON_HOME) {
            break;
        }

        VIDEO_WaitVSync();
    }

    return 0;
}
