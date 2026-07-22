-------------------------------------
Translated Report (Full Report Below)
-------------------------------------

Process:               Live [99818]
Path:                  /Applications/Ableton Live 12 Suite.app/Contents/MacOS/Live
Identifier:            com.ableton.live
Version:               12.3.5 (2026-01-14_c4ac4719dc) (12.3.5 (2026-01-14_c4ac4719dc))
Code Type:             ARM-64 (Native)
Parent Process:        launchd [1]
User ID:               501

Date/Time:             2026-07-20 03:03:09.7723 +0200
OS Version:            macOS 15.5 (24F74)
Report Version:        12
Anonymous UUID:        E406B683-B03C-1202-28CB-EADD5B7C065D

Sleep/Wake UUID:       AEE66E85-840B-40EE-8FF8-04AB92EBF56C

Time Awake Since Boot: 410000 seconds
Time Since Wake:       13604 seconds

System Integrity Protection: enabled

Crashed Thread:        0  MainThread  Dispatch queue: com.apple.main-thread

Exception Type:        EXC_BAD_ACCESS (SIGSEGV)
Exception Codes:       KERN_INVALID_ADDRESS at 0x0000000000000040
Exception Codes:       0x0000000000000001, 0x0000000000000040

Termination Reason:    Namespace SIGNAL, Code 11 Segmentation fault: 11
Terminating Process:   exc handler [99818]

VM Region Info: 0x40 is not in any region.  Bytes before following region: 4339531712
      REGION TYPE                    START - END         [ VSIZE] PRT/MAX SHRMOD  REGION DETAIL
      UNUSED SPACE AT START
--->  
      __TEXT                      102a80000-108cac000    [ 98.2M] r-x/r-x SM=COW  /Applications/Ableton Live 12 Suite.app/Contents/MacOS/Live

Thread 0 Crashed:: MainThread Dispatch queue: com.apple.main-thread
0   Gravel Possibility Distortion 	       0x34b0c01b8 juce::Rectangle<int>::getWidth() const + 12 (juce_Rectangle.h:140)
1   Gravel Possibility Distortion 	       0x34b0fef58 juce::Component::getWidth() const + 28 (juce_Component.h:280)
2   Gravel Possibility Distortion 	       0x34b21e0f8 juce::Component::setBounds(int, int, int, int) + 192 (juce_Component.cpp:848)
3   Gravel Possibility Distortion 	       0x34b204d3c juce::Component::setBounds(juce::Rectangle<int>) + 120 (juce_Component.cpp:961)
4   Gravel Possibility Distortion 	       0x34b0ff098 GravelPossibilityDistortionAudioProcessorEditor::resized() + 308 (PluginEditor.cpp:135)
5   Gravel Possibility Distortion 	       0x34b21e4b8 juce::Component::sendMovedResizedMessages(bool, bool) + 156 (juce_Component.cpp:922)
6   Gravel Possibility Distortion 	       0x34b21e40c juce::Component::sendMovedResizedMessagesIfPending() + 148 (juce_Component.cpp:904)
7   Gravel Possibility Distortion 	       0x34b21e368 juce::Component::setBounds(int, int, int, int) + 816 (juce_Component.cpp:890)
8   Gravel Possibility Distortion 	       0x34b211f8c juce::Component::setSize(int, int) + 76 (juce_Component.cpp:954)
9   Gravel Possibility Distortion 	       0x34b0fdc74 GravelPossibilityDistortionAudioProcessorEditor::GravelPossibilityDistortionAudioProcessorEditor(GravelPossibilityDistortionAudioProcessor&) + 356 (PluginEditor.cpp:74)
10  Gravel Possibility Distortion 	       0x34b0fea2c GravelPossibilityDistortionAudioProcessorEditor::GravelPossibilityDistortionAudioProcessorEditor(GravelPossibilityDistortionAudioProcessor&) + 36 (PluginEditor.cpp:73)
11  Gravel Possibility Distortion 	       0x34b0f6754 GravelPossibilityDistortionAudioProcessor::createEditor() + 44 (PluginProcessor.cpp:110)
12  Gravel Possibility Distortion 	       0x34baa9f10 juce::AudioProcessor::createEditorIfNeeded() + 240 (juce_AudioProcessor.cpp:919)
13  Gravel Possibility Distortion 	       0x34b0b725c juce::JuceVST3EditController::JuceVST3Editor::ContentWrapperComponent::createEditor(juce::AudioProcessor&) + 36 (juce_audio_plugin_client_VST3.cpp:2336)
14  Gravel Possibility Distortion 	       0x34b0b5720 juce::JuceVST3EditController::JuceVST3Editor::createContentWrapperComponentIfNeeded() + 116 (juce_audio_plugin_client_VST3.cpp:2516)
15  Gravel Possibility Distortion 	       0x34b0b5460 juce::JuceVST3EditController::JuceVST3Editor::JuceVST3Editor(juce::JuceVST3EditController&, juce::JuceAudioProcessor&) + 240 (juce_audio_plugin_client_VST3.cpp:1939)
16  Gravel Possibility Distortion 	       0x34b0b5360 juce::JuceVST3EditController::JuceVST3Editor::JuceVST3Editor(juce::JuceVST3EditController&, juce::JuceAudioProcessor&) + 44 (juce_audio_plugin_client_VST3.cpp:1938)
17  Gravel Possibility Distortion 	       0x34b0acef0 juce::JuceVST3EditController::createView(char const*) + 276 (juce_audio_plugin_client_VST3.cpp:1453)
18  Live                          	       0x105ae24b8 0x102a80000 + 50734264
19  Live                          	       0x105ad3f88 0x102a80000 + 50675592
20  Live                          	       0x105ad3d1c 0x102a80000 + 50674972
21  Live                          	       0x105ad3be0 0x102a80000 + 50674656
22  Live                          	       0x105add480 0x102a80000 + 50713728
23  Live                          	       0x105a5fc2c 0x102a80000 + 50199596
24  Live                          	       0x105a5fb5c 0x102a80000 + 50199388
25  Live                          	       0x105a5fcac 0x102a80000 + 50199724
26  Live                          	       0x103f4e690 0x102a80000 + 21816976
27  Live                          	       0x103f586ec 0x102a80000 + 21858028
28  Live                          	       0x105a5fa34 0x102a80000 + 50199092
29  Live                          	       0x105a72414 0x102a80000 + 50275348
30  Live                          	       0x104145c6c 0x102a80000 + 23878764
31  Live                          	       0x104145d98 void ableton::utility::detail::CallbackTypes<TOneShotTimer, void>::CallMemberFunc<&TOneShotTimer::OnTimer()>(void*) + 80
32  Live                          	       0x103e3df60 0x102a80000 + 20701024
33  Live                          	       0x103e3dd14 0x102a80000 + 20700436
34  Live                          	       0x1040b25b8 0x102a80000 + 23274936
35  Foundation                    	       0x18e594fcc __NSFireTimer + 104
36  CoreFoundation                	       0x18cfa6e14 __CFRUNLOOP_IS_CALLING_OUT_TO_A_TIMER_CALLBACK_FUNCTION__ + 32
37  CoreFoundation                	       0x18cfa6ad4 __CFRunLoopDoTimer + 980
38  CoreFoundation                	       0x18cfa6610 __CFRunLoopDoTimers + 332
39  CoreFoundation                	       0x18cf8ca18 __CFRunLoopRun + 1848
40  CoreFoundation                	       0x18cf8bc58 CFRunLoopRunSpecific + 572
41  HIToolbox                     	       0x198a2027c RunCurrentEventLoopInMode + 324
42  HIToolbox                     	       0x198a234e8 ReceiveNextEventCommon + 676
43  HIToolbox                     	       0x198bae484 _BlockUntilNextEventMatchingListInModeWithFilter + 76
44  AppKit                        	       0x190eb3ab4 _DPSNextEvent + 684
45  AppKit                        	       0x1918525b0 -[NSApplication(NSEventRouting) _nextEventMatchingEventMask:untilDate:inMode:dequeue:] + 688
46  AppKit                        	       0x190ea6c64 -[NSApplication run] + 480
47  Live                          	       0x1040b223c 0x102a80000 + 23274044
48  dyld                          	       0x18cb02b98 start + 6076

Thread 1:: NetIoThread
0   libsystem_kernel.dylib        	       0x18ce60bb0 semaphore_wait_trap + 8
1   Live                          	       0x103bbcf68 0x102a80000 + 18075496
2   Live                          	       0x103fa04a4 void ableton::utility::detail::CallbackTypes<ANetIoThread, void, TUserData>::CallMemberFunc<&ANetIoThread::Main(TUserData)>(void*, TUserData) + 100
3   Live                          	       0x103bd4d14 0x102a80000 + 18173204
4   Live                          	       0x103bd5a34 0x102a80000 + 18176564
5   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
6   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 2:
0   libsystem_kernel.dylib        	       0x18ce60c34 mach_msg2_trap + 8
1   libsystem_kernel.dylib        	       0x18ce733a0 mach_msg2_internal + 76
2   libsystem_kernel.dylib        	       0x18ce69764 mach_msg_overwrite + 484
3   libsystem_kernel.dylib        	       0x18ce60fa8 mach_msg + 24
4   CoreMIDI                      	       0x1a951deac XServerMachPort::ReceiveMessage(int&, void*, int&) + 104
5   CoreMIDI                      	       0x1a953f740 MIDIProcess::MIDIInPortThread::Run() + 148
6   CoreMIDI                      	       0x1a953476c CADeprecated::XThread::RunHelper(void*) + 48
7   CoreMIDI                      	       0x1a953ee44 CADeprecated::CAPThread::Entry(CADeprecated::CAPThread*) + 96
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 3:: FileThread
0   libsystem_kernel.dylib        	       0x18ce60bb0 semaphore_wait_trap + 8
1   Live                          	       0x103bbcf68 0x102a80000 + 18075496
2   Live                          	       0x104b87a90 0x102a80000 + 34634384
3   Live                          	       0x103bd4d14 0x102a80000 + 18173204
4   Live                          	       0x103bd5a34 0x102a80000 + 18176564
5   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
6   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 4:: IPC Channel Reader
0   libsystem_kernel.dylib        	       0x18ce69498 poll + 8
1   Live                          	       0x106a6d5e8 0x102a80000 + 67032552
2   Live                          	       0x103fcadcc 0x102a80000 + 22326732
3   Live                          	       0x103fcb27c 0x102a80000 + 22327932
4   Live                          	       0x103fcc93c 0x102a80000 + 22333756
5   Live                          	       0x103fcc4f0 0x102a80000 + 22332656
6   Live                          	       0x103bd4d14 0x102a80000 + 18173204
7   Live                          	       0x103bd5a34 0x102a80000 + 18176564
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 5:: caulk.messenger.shared:17
0   libsystem_kernel.dylib        	       0x18ce60bb0 semaphore_wait_trap + 8
1   caulk                         	       0x198507cc8 caulk::semaphore::timed_wait(double) + 224
2   caulk                         	       0x198507b70 caulk::concurrent::details::worker_thread::run() + 32
3   caulk                         	       0x198507844 void* caulk::thread_proxy<std::__1::tuple<caulk::thread::attributes, void (caulk::concurrent::details::worker_thread::*)(), std::__1::tuple<caulk::concurrent::details::worker_thread*>>>(void*) + 96
4   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
5   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 6:: caulk.messenger.shared:high
0   libsystem_kernel.dylib        	       0x18ce60bb0 semaphore_wait_trap + 8
1   caulk                         	       0x198507cc8 caulk::semaphore::timed_wait(double) + 224
2   caulk                         	       0x198507b70 caulk::concurrent::details::worker_thread::run() + 32
3   caulk                         	       0x198507844 void* caulk::thread_proxy<std::__1::tuple<caulk::thread::attributes, void (caulk::concurrent::details::worker_thread::*)(), std::__1::tuple<caulk::concurrent::details::worker_thread*>>>(void*) + 96
4   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
5   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 7:
0   libsystem_kernel.dylib        	       0x18ce66d04 kevent + 8
1   Live                          	       0x106a833d8 0x102a80000 + 67122136
2   Live                          	       0x106a6b04c 0x102a80000 + 67022924
3   Live                          	       0x106a82e50 0x102a80000 + 67120720
4   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
5   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 8:
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3298 std::__1::condition_variable::wait(std::__1::unique_lock<std::__1::mutex>&) + 32
3   Live                          	       0x105c2d184 0x102a80000 + 52089220
4   Live                          	       0x10799750c 0x102a80000 + 82933004
5   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
6   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 9:: browser
0   libsystem_kernel.dylib        	       0x18ce60bb0 semaphore_wait_trap + 8
1   Live                          	       0x106a67cb0 0x102a80000 + 67009712
2   Live                          	       0x106741620 ableton::live_library::AsyncLibraryHub::Impl::workerThreadMain(ableton::live_library::AsyncLibraryHub::Impl*) + 380
3   Live                          	       0x106741e3c 0x102a80000 + 63708732
4   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
5   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 10:
0   libsystem_kernel.dylib        	       0x18ce66d04 kevent + 8
1   Live                          	       0x10384a734 0x102a80000 + 14460724
2   Live                          	       0x10384b894 0x102a80000 + 14465172
3   Live                          	       0x10384b578 0x102a80000 + 14464376
4   Live                          	       0x103851970 0x102a80000 + 14489968
5   Live                          	       0x1038518bc 0x102a80000 + 14489788
6   Live                          	       0x1038517fc 0x102a80000 + 14489596
7   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
8   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 11:: JUCE v8.0.12: Timer
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   PicoSmp                       	       0x15da60644 std::__1::cv_status std::__1::condition_variable::wait_for<long long, std::__1::ratio<1l, 1000000000l>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>> const&) + 388
4   PicoSmp                       	       0x15da6028c void std::__1::condition_variable::__do_timed_wait[abi:ne190102]<std::__1::chrono::steady_clock>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
5   PicoSmp                       	       0x15da821b0 std::__1::cv_status std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&) + 160
6   PicoSmp                       	       0x15da82040 bool std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&, juce::WaitableEvent::wait(double) const::$_1) + 72
7   PicoSmp                       	       0x15da22a98 bool std::__1::condition_variable::wait_for[abi:ne190102]<double, std::__1::ratio<1l, 1000l>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000l>> const&, juce::WaitableEvent::wait(double) const::$_1) + 100
8   PicoSmp                       	       0x15d9f4330 juce::WaitableEvent::wait(double) const + 188
9   PicoSmp                       	       0x15d996464 juce::Timer::TimerThread::run() + 340
10  PicoSmp                       	       0x15d9f4e78 juce::Thread::threadEntryPoint() + 244
11  PicoSmp                       	       0x15d9f516c juce::juce_threadEntryPoint(void*) + 24
12  PicoSmp                       	       0x15da5094c juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::operator()(void*) const + 148
13  PicoSmp                       	       0x15da508ac juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::__invoke(void*) + 28
14  libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
15  libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 12:: sentry-http
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   SpliceAbletonLive             	       0x14fa5b5a4 0x14f200000 + 8762788
3   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
4   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 13:: JUCE Timer
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f6160d4 0x14f200000 + 4284628
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 14:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 15:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 16:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 17:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 18:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 19:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 20:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 21:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 22:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 23:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 24:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 25:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 26:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 27:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 28:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 29:: Pool
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SpliceAbletonLive             	       0x14f391fb8 0x14f200000 + 1646520
4   SpliceAbletonLive             	       0x14f315b30 0x14f200000 + 1137456
5   SpliceAbletonLive             	       0x14f365134 0x14f200000 + 1462580
6   SpliceAbletonLive             	       0x14f3167c4 0x14f200000 + 1140676
7   SpliceAbletonLive             	       0x14f3880fc 0x14f200000 + 1605884
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 30:
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3298 std::__1::condition_variable::wait(std::__1::unique_lock<std::__1::mutex>&) + 32
3   MaxPlugLib                    	       0x32d817e78 spdlog::details::mpmc_blocking_queue<spdlog::details::async_msg>::dequeue(spdlog::details::async_msg&) + 72
4   MaxPlugLib                    	       0x32d817d64 spdlog::details::thread_pool::process_next_msg_() + 124
5   MaxPlugLib                    	       0x32d817b78 void* std::__1::__thread_proxy[abi:v160006]<std::__1::tuple<std::__1::unique_ptr<std::__1::__thread_struct, std::__1::default_delete<std::__1::__thread_struct>>, spdlog::details::thread_pool::thread_pool(unsigned long, unsigned long, std::__1::function<void ()>, std::__1::function<void ()>)::'lambda'()>>(void*) + 76
6   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
7   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 31:: timer
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   MaxPlugLib                    	       0x32d33c0ac std::__1::cv_status std::__1::condition_variable_any::wait_until<(anonymous namespace)::SysthreadMutexAdapter, std::__1::chrono::steady_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>((anonymous namespace)::SysthreadMutexAdapter&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>> const&) + 276
4   MaxPlugLib                    	       0x32d33bd84 (anonymous namespace)::Systimer::threadfun() + 256
5   MaxPlugLib                    	       0x32d33c220 void* std::__1::__thread_proxy[abi:v160006]<std::__1::tuple<std::__1::unique_ptr<std::__1::__thread_struct, std::__1::default_delete<std::__1::__thread_struct>>, void ((anonymous namespace)::Systimer::*)(), (anonymous namespace)::Systimer*>>(void*) + 72
6   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
7   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 32:: com.apple.NSEventThread
0   libsystem_kernel.dylib        	       0x18ce60c34 mach_msg2_trap + 8
1   libsystem_kernel.dylib        	       0x18ce733a0 mach_msg2_internal + 76
2   libsystem_kernel.dylib        	       0x18ce69764 mach_msg_overwrite + 484
3   libsystem_kernel.dylib        	       0x18ce60fa8 mach_msg + 24
4   CoreFoundation                	       0x18cf8de7c __CFRunLoopServiceMachPort + 160
5   CoreFoundation                	       0x18cf8c798 __CFRunLoopRun + 1208
6   CoreFoundation                	       0x18cf8bc58 CFRunLoopRunSpecific + 572
7   AppKit                        	       0x190fd77fc _NSEventThread + 140
8   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
9   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 33:: JUCE Timer
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   MaxPlugLib                    	       0x32d632820 juce::WaitableEvent::wait(double) const + 544
4   MaxPlugLib                    	       0x32d667398 juce::Timer::TimerThread::run() + 476
5   MaxPlugLib                    	       0x32d6333ec juce::Thread::threadEntryPoint() + 292
6   MaxPlugLib                    	       0x32d655dc4 juce::Thread::createNativeThread(juce::Thread::Priority)::$_28::__invoke(void*) + 152
7   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
8   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 34:: tcp_listener
0   libsystem_kernel.dylib        	       0x18ce6946c __accept + 8
1   MaxPlugLib                    	       0x32d62461c juce::StreamingSocket::waitForNextConnection() const + 92
2   MaxPlugLib                    	       0x32d3edc24 tcpconnection_listenthread(_tcpconnection*) + 168
3   MaxPlugLib                    	       0x32d2e95f4 systhread_threadproc + 232
4   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
5   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 35:: caulk::deferred_logger
0   libsystem_kernel.dylib        	       0x18ce60bb0 semaphore_wait_trap + 8
1   caulk                         	       0x198507cc8 caulk::semaphore::timed_wait(double) + 224
2   caulk                         	       0x198507b70 caulk::concurrent::details::worker_thread::run() + 32
3   caulk                         	       0x198507844 void* caulk::thread_proxy<std::__1::tuple<caulk::thread::attributes, void (caulk::concurrent::details::worker_thread::*)(), std::__1::tuple<caulk::concurrent::details::worker_thread*>>>(void*) + 96
4   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
5   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 36:: AudioCalc
0   libsystem_kernel.dylib        	       0x18ce60bb0 semaphore_wait_trap + 8
1   Live                          	       0x103bbcf68 0x102a80000 + 18075496
2   Live                          	       0x10440d3bc 0x102a80000 + 26792892
3   Live                          	       0x103bd4d14 0x102a80000 + 18173204
4   Live                          	       0x103bd5a34 0x102a80000 + 18176564
5   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
6   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 37:: AudioCalc
0   libsystem_kernel.dylib        	       0x18ce60bb0 semaphore_wait_trap + 8
1   Live                          	       0x103bbcf68 0x102a80000 + 18075496
2   Live                          	       0x10440d3bc 0x102a80000 + 26792892
3   Live                          	       0x103bd4d14 0x102a80000 + 18173204
4   Live                          	       0x103bd5a34 0x102a80000 + 18176564
5   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
6   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 38:: AudioCalc
0   libsystem_kernel.dylib        	       0x18ce60bb0 semaphore_wait_trap + 8
1   Live                          	       0x103bbcf68 0x102a80000 + 18075496
2   Live                          	       0x10440d3bc 0x102a80000 + 26792892
3   Live                          	       0x103bd4d14 0x102a80000 + 18173204
4   Live                          	       0x103bd5a34 0x102a80000 + 18176564
5   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
6   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 39:: JUCE Timer
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SmackTheDrums                 	       0x17c63cb88 std::__1::cv_status std::__1::condition_variable::wait_for<long long, std::__1::ratio<1l, 1000000000l>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>> const&) + 388
4   SmackTheDrums                 	       0x17c63c7f8 void std::__1::condition_variable::__do_timed_wait[abi:ne190102]<std::__1::chrono::steady_clock>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
5   SmackTheDrums                 	       0x17c6d950c std::__1::cv_status std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&) + 160
6   SmackTheDrums                 	       0x17c6d939c bool std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&, juce::WaitableEvent::wait(double) const::$_1) + 72
7   SmackTheDrums                 	       0x17c5b3c40 bool std::__1::condition_variable::wait_for[abi:ne190102]<double, std::__1::ratio<1l, 1000l>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000l>> const&, juce::WaitableEvent::wait(double) const::$_1) + 100
8   SmackTheDrums                 	       0x17c5845a8 juce::WaitableEvent::wait(double) const + 188
9   SmackTheDrums                 	       0x17c586014 juce::Thread::wait(double) const + 36
10  SmackTheDrums                 	       0x17c527ff4 juce::Timer::TimerThread::run() + 420
11  SmackTheDrums                 	       0x17c5850f0 juce::Thread::threadEntryPoint() + 244
12  SmackTheDrums                 	       0x17c5853e4 juce::juce_threadEntryPoint(void*) + 24
13  SmackTheDrums                 	       0x17c62d2d8 juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::operator()(void*) const + 148
14  SmackTheDrums                 	       0x17c62d238 juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::__invoke(void*) + 28
15  libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
16  libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 40:: SmackSampleLoader
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   SmackTheDrums                 	       0x17c63cb88 std::__1::cv_status std::__1::condition_variable::wait_for<long long, std::__1::ratio<1l, 1000000000l>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>> const&) + 388
4   SmackTheDrums                 	       0x17c63c7f8 void std::__1::condition_variable::__do_timed_wait[abi:ne190102]<std::__1::chrono::steady_clock>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
5   SmackTheDrums                 	       0x17c6d950c std::__1::cv_status std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&) + 160
6   SmackTheDrums                 	       0x17c6d939c bool std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&, juce::WaitableEvent::wait(double) const::$_1) + 72
7   SmackTheDrums                 	       0x17c5b3c40 bool std::__1::condition_variable::wait_for[abi:ne190102]<double, std::__1::ratio<1l, 1000l>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000l>> const&, juce::WaitableEvent::wait(double) const::$_1) + 100
8   SmackTheDrums                 	       0x17c5845a8 juce::WaitableEvent::wait(double) const + 188
9   SmackTheDrums                 	       0x17c586014 juce::Thread::wait(double) const + 36
10  SmackTheDrums                 	       0x17bb333dc SmackTheDrumsAudioProcessor::SampleLoaderThread::run() + 244
11  SmackTheDrums                 	       0x17c5850f0 juce::Thread::threadEntryPoint() + 244
12  SmackTheDrums                 	       0x17c5853e4 juce::juce_threadEntryPoint(void*) + 24
13  SmackTheDrums                 	       0x17c62d2d8 juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::operator()(void*) const + 148
14  SmackTheDrums                 	       0x17c62d238 juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::__invoke(void*) + 28
15  libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
16  libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 41:: com.apple.audio.IOThread.client
0   libsystem_kernel.dylib        	       0x18ce60bbc semaphore_wait_signal_trap + 8
1   caulk                         	       0x1985242f8 caulk::mach::semaphore::wait_signal_or_error(caulk::mach::semaphore&) + 36
2   CoreAudio                     	       0x18fd92f58 HALC_ProxyIOContext::IOWorkLoop() + 5276
3   CoreAudio                     	       0x18fd913a8 invocation function for block in HALC_ProxyIOContext::HALC_ProxyIOContext(unsigned int, unsigned int) + 172
4   CoreAudio                     	       0x18ff3b37c HALC_IOThread::Entry(void*) + 88
5   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
6   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 42:: CVDisplayLink
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea310c _pthread_cond_wait + 1028
2   CoreVideo                     	       0x1968371f4 CVDisplayLink::waitUntil(unsigned long long) + 336
3   CoreVideo                     	       0x1968362dc CVDisplayLink::runIOThread() + 500
4   libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
5   libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8

Thread 43:
0   libsystem_pthread.dylib       	       0x18ce9db6c start_wqthread + 0

Thread 44:
0   libsystem_pthread.dylib       	       0x18ce9db6c start_wqthread + 0

Thread 45:
0   libsystem_pthread.dylib       	       0x18ce9db6c start_wqthread + 0

Thread 46:
0   libsystem_pthread.dylib       	       0x18ce9db6c start_wqthread + 0

Thread 47:
0   libsystem_pthread.dylib       	       0x18ce9db6c start_wqthread + 0

Thread 48:
0   libsystem_pthread.dylib       	       0x18ce9db6c start_wqthread + 0

Thread 49:: JUCE v8.0.12: Timer
0   libsystem_kernel.dylib        	       0x18ce643cc __psynch_cvwait + 8
1   libsystem_pthread.dylib       	       0x18cea30e0 _pthread_cond_wait + 984
2   libc++.1.dylib                	       0x18cdd3330 std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104
3   Gravel Possibility Distortion 	       0x34ba3db2c std::__1::cv_status std::__1::condition_variable::wait_for<long long, std::__1::ratio<1l, 1000000000l>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>> const&) + 328 (condition_variable.h:196)
4   Gravel Possibility Distortion 	       0x34ba3d7fc void std::__1::condition_variable::__do_timed_wait[abi:ne190102]<std::__1::chrono::steady_clock>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>) + 104 (condition_variable.h:235)
5   Gravel Possibility Distortion 	       0x34ba760f4 std::__1::cv_status std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&) + 132 (condition_variable.h:161)
6   Gravel Possibility Distortion 	       0x34ba75fa0 bool std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&, juce::WaitableEvent::wait(double) const::$_1) + 72 (condition_variable.h:169)
7   Gravel Possibility Distortion 	       0x34ba058a8 bool std::__1::condition_variable::wait_for[abi:ne190102]<double, std::__1::ratio<1l, 1000l>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000l>> const&, juce::WaitableEvent::wait(double) const::$_1) + 100 (condition_variable.h:205)
8   Gravel Possibility Distortion 	       0x34b9d43dc juce::WaitableEvent::wait(double) const + 188 (juce_WaitableEvent.cpp:55)
9   Gravel Possibility Distortion 	       0x34b9d6224 juce::Thread::wait(double) const + 36 (juce_Thread.cpp:302)
10  Gravel Possibility Distortion 	       0x34b96bb8c juce::Timer::TimerThread::run() + 420 (juce_Timer.cpp:144)
11  Gravel Possibility Distortion 	       0x34b9d51bc juce::Thread::threadEntryPoint() + 340 (juce_Thread.cpp:110)
12  Gravel Possibility Distortion 	       0x34b9d5580 juce::juce_threadEntryPoint(void*) + 24 (juce_Thread.cpp:132)
13  Gravel Possibility Distortion 	       0x34ba2cca0 juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::operator()(void*) const + 148 (juce_Threads_mac.mm:159)
14  Gravel Possibility Distortion 	       0x34ba2cc00 juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::__invoke(void*) + 28 (juce_Threads_mac.mm:143)
15  libsystem_pthread.dylib       	       0x18cea2c0c _pthread_start + 136
16  libsystem_pthread.dylib       	       0x18ce9db80 thread_start + 8


Thread 0 crashed with ARM Thread State (64-bit):
    x0: 0x0000000000000038   x1: 0x00000000000000aa   x2: 0x0000000000000050   x3: 0x0000000000000083
    x4: 0x000000000000015c   x5: 0x0000010000000200   x6: 0x0000000000000000   x7: 0x000060000112d034
    x8: 0x0000000000000038   x9: 0x0000000000000000  x10: 0x00000001fafb1f00  x11: 0x0000000000000001
   x12: 0x00000000fffffffd  x13: 0x0000000000000000  x14: 0x0000000000000000  x15: 0x0000000000000000
   x16: 0x00000001fafb1f00  x17: 0x00000001fafb1f00  x18: 0x0000000000000000  x19: 0x000000034b0adf48
   x20: 0x00000001083df62a  x21: 0x0000600002926990  x22: 0x000000010907e6c8  x23: 0x0000000000000001
   x24: 0x00000001096d9a40  x25: 0x00000001096d9a40  x26: 0x00000000211200e5  x27: 0x00000922b72df432
   x28: 0x000000018e593808   fp: 0x000000016d37d270   lr: 0x000000034b0fef58
    sp: 0x000000016d37d250   pc: 0x000000034b0c01b8 cpsr: 0x60001000
   far: 0x0000000000000040  esr: 0x92000006 (Data Abort) byte read Translation fault

Binary Images:
       0x102a80000 -        0x108cabfff com.ableton.live (12.3.5 (2026-01-14_c4ac4719dc)) <22acf5c7-4011-382b-a5d0-b782292a7879> /Applications/Ableton Live 12 Suite.app/Contents/MacOS/Live
       0x10e310000 -        0x10ee93fff libonnxruntime_abl.dylib (*) <94d05e19-a04a-30b0-aa54-8db26de5faec> /Applications/Ableton Live 12 Suite.app/Contents/Frameworks/libonnxruntime_abl.dylib
       0x10b800000 -        0x10b817fff libusb-1.0.dylib (*) <f683466a-2a35-31eb-9bba-4d25d5dd42f4> /Applications/Ableton Live 12 Suite.app/Contents/Frameworks/libusb-1.0.dylib
       0x10ba44000 -        0x10ba8bfff se.propellerheads.rex.library (*) <ddfec8a2-ead4-3fab-9674-1be6f6e189c9> /Applications/Ableton Live 12 Suite.app/Contents/Frameworks/REX Shared Library.framework/Versions/A/REX Shared Library
       0x10e0bc000 -        0x10e0c7fff libobjc-trampolines.dylib (*) <d02a05cb-6440-3e7e-a02f-931734cab666> /usr/lib/libobjc-trampolines.dylib
       0x11d100000 -        0x11d807fff com.apple.AGXMetalG14G (327.5) <ecba96b8-21ca-3241-ba32-76ebceb61cbd> /System/Library/Extensions/AGXMetalG14G.bundle/Contents/MacOS/AGXMetalG14G
       0x158000000 -        0x158ac7fff com.Riverb.RiverbPrototype (0.1.0) <7920faa6-b277-3397-a8d7-35d10321458e> /Users/USER/Library/Audio/Plug-Ins/VST3/Riverb Prototype.vst3/Contents/MacOS/Riverb Prototype
       0x15d100000 -        0x15de93fff com.thievery.picosmp (0.1.0) <6abc06a5-4c20-387b-bf08-9650ac0a02f7> /Users/USER/Library/Audio/Plug-Ins/Components/PicoSmp.component/Contents/MacOS/PicoSmp
       0x13f114000 -        0x13f153fff com.ableton.collision (1.0) <7e35b71f-0b92-30ef-a767-1d9b80ed6f21> /Applications/Ableton Live 12 Suite.app/Contents/App-Resources/Devices/Mac/Collision.bundle/Contents/MacOS/Collision
       0x13b2c0000 -        0x13b2dffff com.apple.security.csparser (3.0) <c12848ee-0663-3987-842f-8832599d139f> /System/Library/Frameworks/Security.framework/Versions/A/PlugIns/csparser.bundle/Contents/MacOS/csparser
       0x14f200000 -        0x14fd47fff com.splice.ableton-live-integration (1.0.1) <d2d0539f-e28d-37d0-a504-cf7cd2ab8414> /Users/USER/Library/Application Support/Ableton/*/SpliceAbletonLive
       0x13b730000 -        0x13b733fff com.cycling74.maxchecker (1.01) <9b18bb58-3a99-36a2-b318-5b8f6cc06142> /Applications/Max.app/Contents/Resources/C74/support/maxchecker.bundle/Contents/MacOS/maxchecker
       0x32cf70000 -        0x32f873fff com.cycling74.MaxPlugLib ( (07b9c4cbe57)) <1ca7d171-79c9-3ddf-ad38-944ca567007a> /Applications/Max.app/Contents/Resources/C74/support/MaxPlugLib.bundle/Contents/MacOS/MaxPlugLib
       0x13b744000 -        0x13b7bbfff com.cycling74.MaxAudioAPIImpl (*) <a6fc16bd-6192-3bb6-b597-ed86c21fe6a2> /Applications/Max.app/Contents/Frameworks/MaxAudioAPIImpl.framework/Versions/A/MaxAudioAPIImpl
       0x14b544000 -        0x14b5cffff com.cycling74.MaxLuaImpl (*) <a9399ad5-d95c-3fcc-937f-b83e8ebff0bf> /Applications/Max.app/Contents/Frameworks/MaxLuaImpl.framework/Versions/A/MaxLuaImpl
       0x159cc0000 -        0x159dd3fff com.cycling74.JitterAPIImpl (1.7.0) <ffa7ae9f-f3d0-3a71-a250-0058dc2c6107> /Applications/Max.app/Contents/Frameworks/JitterAPIImpl.framework/Versions/A/JitterAPIImpl
       0x13b720000 -        0x13b723fff com.cycling74.MaxLua (*) <36c7a249-8d04-3da8-8bf0-e0d50dbd63d2> /Applications/Ableton Live 12 Suite.app/Contents/App-Resources/Max/Max.app/Contents/Frameworks/MaxLua.framework/Versions/A/MaxLua
       0x1599b0000 -        0x159aeffff libmozjs185_impl.dylib (*) <919cbda1-301b-36a8-877b-dd0e4552c71c> /Applications/Max.app/Contents/Frameworks/libmozjs185_impl.dylib
       0x13f168000 -        0x13f193fff com.cycling74.patcher (9.1.2 (07b9c4cbe57)) <daaf82b9-3937-320a-9317-7d0bb1cac0a8> /Applications/Max.app/Contents/Resources/C74/support/patcher.bundle/Contents/MacOS/patcher
       0x13b7f4000 -        0x13b7f7fff com.cycling74.MaxAudioAPI (*) <6f31fc51-af05-3315-adc3-d165cc3c14bb> /Applications/Ableton Live 12 Suite.app/Contents/App-Resources/Max/Max.app/Contents/Frameworks/MaxAudioAPI.framework/Versions/A/MaxAudioAPI
       0x13f1e8000 -        0x13f1ebfff com.cycling74.fseventwatcher (9.1.2 (07b9c4cbe57)) <72060850-0acf-3dba-9496-e39f9edce054> /Applications/Max.app/Contents/Resources/C74/extensions/max/fseventwatcher.mxo/Contents/MacOS/fseventwatcher
       0x15b3b8000 -        0x15b3bbfff com.cycling74.cefaudio (9.1.2 (07b9c4cbe57)) <7b9d6986-da00-3351-91fe-9417c39871ce> /Applications/Max.app/Contents/Resources/C74/extensions/msp/cefaudio.mxo/Contents/MacOS/cefaudio
       0x15b3cc000 -        0x15b503fff com.cycling74.gl2 (9.1.2 (07b9c4cbe57)) <4ce3b6f8-b62a-3cd9-80d4-6edef5857e57> /Applications/Max.app/Contents/Resources/C74/extensions/jitter/gl2.mxo/Contents/MacOS/gl2
       0x159ef0000 -        0x159ef3fff com.cycling74.JitterAPI (1.7.0) <871ecfe7-6d82-36d4-ba8f-5fd5f42352ef> /Applications/Ableton Live 12 Suite.app/Contents/App-Resources/Max/Max.app/Contents/Frameworks/JitterAPI.framework/Versions/A/JitterAPI
       0x15f278000 -        0x15f31bfff com.cycling74.sketch (9.1.2 (07b9c4cbe57)) <08eec072-eea7-3212-bc06-f1731b9745d4> /Applications/Max.app/Contents/Resources/C74/extensions/jitter/sketch.mxo/Contents/MacOS/sketch
       0x15b56c000 -        0x15b5a3fff com.cycling74.live.guilib (9.1.2 (07b9c4cbe57)) <5bf57c30-c8d0-31f7-b7b2-7f9984a86a8c> /Applications/Max.app/Contents/Resources/C74/extensions/m4l/live.guilib.mxo/Contents/MacOS/live.guilib
       0x159ed4000 -        0x159edbfff com.cycling74.autohelp (9.1.2 (07b9c4cbe57)) <855f3ba2-e29d-38e4-a575-ccdc196a05b9> /Applications/Max.app/Contents/Resources/C74/extensions/max/autohelp.mxo/Contents/MacOS/autohelp
       0x15b5ec000 -        0x15b5effff com.cycling74.debugwindow (9.1.2 (07b9c4cbe57)) <e622c0b5-9c77-3944-b019-769890117926> /Applications/Max.app/Contents/Resources/C74/extensions/max/debugwindow.mxo/Contents/MacOS/debugwindow
       0x15f0d4000 -        0x15f1bffff com.cycling74.genpatcher (9.1.2 (07b9c4cbe57)) <4cec7efd-f23b-3bef-a297-21bc1121aba1> /Applications/Max.app/Contents/Resources/C74/extensions/max/genpatcher.mxo/Contents/MacOS/genpatcher
       0x15f230000 -        0x15f233fff com.cycling74.jsrepl (9.1.2 (07b9c4cbe57)) <118e6c16-8e04-3d63-855d-15db0cce3f68> /Applications/Max.app/Contents/Resources/C74/extensions/max/jsrepl.mxo/Contents/MacOS/jsrepl
       0x39dd84000 -        0x3a0ff3fff com.cycling74.maxclang (9.1.2 (07b9c4cbe57)) <05cd9a5a-6136-3382-84e0-3bdfd3785203> /Applications/Max.app/Contents/Resources/C74/extensions/max/maxclang.mxo/Contents/MacOS/maxclang
       0x15f4d0000 -        0x15f563fff com.cycling74.maxurl (9.1.2 (07b9c4cbe57)) <232b3b79-a404-3e55-a418-9d902ad23a35> /Applications/Max.app/Contents/Resources/C74/extensions/max/maxurl.mxo/Contents/MacOS/maxurl
       0x3987dc000 -        0x39a6a7fff com.cycling74.maxxslt (9.1.2 (07b9c4cbe57)) <6c320aa5-d3ce-39b6-ad64-d5c22070b2c0> /Applications/Max.app/Contents/Resources/C74/extensions/max/maxxslt.mxo/Contents/MacOS/maxxslt
       0x15b5c0000 -        0x15b5d7fff com.cycling74.maxzlib (9.1.2 (07b9c4cbe57)) <ba8d17c3-f593-35ba-8395-91017cd19345> /Applications/Max.app/Contents/Resources/C74/extensions/max/maxzlib.mxo/Contents/MacOS/maxzlib
       0x15f34c000 -        0x15f34ffff com.cycling74.objectview (9.1.2 (07b9c4cbe57)) <7ef05977-4987-3bea-9370-4fe1dc795b56> /Applications/Max.app/Contents/Resources/C74/extensions/max/objectview.mxo/Contents/MacOS/objectview
       0x15f360000 -        0x15f367fff com.cycling74.palblocks (9.1.2 (07b9c4cbe57)) <f76d3b3d-3aa5-3042-9603-372b5eb1089e> /Applications/Max.app/Contents/Resources/C74/extensions/max/palblocks.mxo/Contents/MacOS/palblocks
       0x15f378000 -        0x15f37ffff com.cycling74.paramview (9.1.2 (07b9c4cbe57)) <55c716da-2d69-32ff-97f0-5cf5f5596d82> /Applications/Max.app/Contents/Resources/C74/extensions/max/paramview.mxo/Contents/MacOS/paramview
       0x15f244000 -        0x15f24ffff com.cycling74.pianoroll (9.1.2 (07b9c4cbe57)) <2e319224-9b52-3bc6-9208-f74830a98676> /Applications/Max.app/Contents/Resources/C74/extensions/max/pianoroll.mxo/Contents/MacOS/pianoroll
       0x15f260000 -        0x15f267fff com.cycling74.querylib (9.1.2 (07b9c4cbe57)) <9df77bbd-f9c1-3356-a049-854c4cb5ed90> /Applications/Max.app/Contents/Resources/C74/extensions/max/querylib.mxo/Contents/MacOS/querylib
       0x15f3c4000 -        0x15f3cbfff com.cycling74.setplugpath (9.1.2 (07b9c4cbe57)) <84cba2c6-aff8-3a8a-ba12-a72756073c3b> /Applications/Max.app/Contents/Resources/C74/extensions/max/setplugpath.mxo/Contents/MacOS/setplugpath
       0x15f58c000 -        0x15f6d7fff com.cycling74.sqlite (9.1.2 (07b9c4cbe57)) <7d18cf4b-08e6-3fb8-a5c4-0ac855f40a38> /Applications/Max.app/Contents/Resources/C74/extensions/max/sqlite.mxo/Contents/MacOS/sqlite
       0x15f470000 -        0x15f49ffff com.cycling74.synophrys (9.1.2 (07b9c4cbe57)) <31c870d6-0e2b-31d3-bdda-a16e426e2380> /Applications/Max.app/Contents/Resources/C74/extensions/max/synophrys.mxo/Contents/MacOS/synophrys
       0x39a730000 -        0x39b6b7fff com.cycling74.v8 (9.1.2 (07b9c4cbe57)) <07e0ae40-dc0e-3406-9a1d-16b98ff8379e> /Applications/Max.app/Contents/Resources/C74/extensions/max/v8.mxo/Contents/MacOS/v8
       0x15f3dc000 -        0x15f40bfff com.cycling74.yaml (9.1.2 (07b9c4cbe57)) <953e2cbf-4d8d-366e-8706-9d57d6aa68d5> /Applications/Max.app/Contents/Resources/C74/extensions/max/yaml.mxo/Contents/MacOS/yaml
       0x15f434000 -        0x15f437fff com.cycling74.zoomer (9.1.2 (07b9c4cbe57)) <0e79b788-c0a4-3d36-bfca-dd74f76f6f50> /Applications/Max.app/Contents/Resources/C74/extensions/max/zoomer.mxo/Contents/MacOS/zoomer
       0x15f390000 -        0x15f39bfff com.cycling74.max-tilde (9.1.2 (07b9c4cbe57)) <fe5d271f-f544-356a-aac4-16ed5c6d955f> /Applications/Max.app/Contents/Resources/C74/extensions/msp/max~.mxo/Contents/MacOS/max~
       0x15f448000 -        0x15f44ffff com.cycling74.patcherdriver (9.1.2 (07b9c4cbe57)) <1cc20ada-41de-3fe2-8558-d212efe913df> /Applications/Max.app/Contents/Resources/C74/extensions/msp/patcherdriver.mxo/Contents/MacOS/patcherdriver
       0x15f748000 -        0x15f753fff com.cycling74.polybuffer (9.1.2 (07b9c4cbe57)) <f952f6ab-e0cf-39e7-bc9b-d3466502f7a6> /Applications/Max.app/Contents/Resources/C74/extensions/msp/polybuffer.mxo/Contents/MacOS/polybuffer
       0x15f3b0000 -        0x15f3b3fff com.cycling74.probe.history-tilde (9.1.2 (07b9c4cbe57)) <e261800b-7175-3d50-b4d6-4bbc735d25fd> /Applications/Max.app/Contents/Resources/C74/extensions/msp/probe.history~.mxo/Contents/MacOS/probe.history~
       0x15f4b8000 -        0x15f4bbfff com.cycling74.probe.meter-tilde (9.1.2 (07b9c4cbe57)) <6f1e5b80-6f1f-38cc-82cb-09e26c39ac8c> /Applications/Max.app/Contents/Resources/C74/extensions/msp/probe.meter~.mxo/Contents/MacOS/probe.meter~
       0x15f764000 -        0x15f767fff com.cycling74.probe.scope-tilde (9.1.2 (07b9c4cbe57)) <c8820ecd-d08b-35a5-b035-3569d2efad6d> /Applications/Max.app/Contents/Resources/C74/extensions/msp/probe.scope~.mxo/Contents/MacOS/probe.scope~
       0x1685ac000 -        0x16874ffff com.cycling74.gl3 (9.1.2 (07b9c4cbe57)) <75ba291c-ce3f-3723-a75f-eae1c735d4d4> /Applications/Max.app/Contents/Resources/C74/packages/Jitter Tools/extensions/gl3.mxo/Contents/MacOS/gl3
       0x15f730000 -        0x15f733fff com.acme.${PRODUCT_NAME:rfc1034identifier} (8.2.0) <a76b9648-661b-3884-b181-ef5485a19614> /Applications/Max.app/Contents/Resources/C74/packages/max-mxj/extensions/mxj_safe.mxo/Contents/MacOS/mxj_safe
       0x1687ec000 -        0x168b97fff com.cycling74.rnbopatcher (*) <b6f1e89d-b9c8-3faf-9b23-0ee1d7050007> /Applications/Max.app/Contents/Resources/C74/packages/RNBO/extensions/rnbopatcher.mxo/Contents/MacOS/rnbopatcher
       0x15f70c000 -        0x15f713fff com.cycling74.comment (9.1.2 (07b9c4cbe57)) <c6c6071c-3717-3185-b207-921c7afcddfb> /Applications/Max.app/Contents/Resources/C74/externals/max/comment.mxo/Contents/MacOS/comment
       0x15f7cc000 -        0x15f7dffff com.cycling74.sysaudio (9.1.2 (07b9c4cbe57)) <a7f77d78-9ca5-3bb6-939a-cbce2f0009da> /Applications/Max.app/Contents/Resources/C74/packages/VIDDLL/extensions/sysaudio.mxo/Contents/MacOS/sysaudio
       0x15f778000 -        0x15f797fff com.cycling74.js (9.1.2 (07b9c4cbe57)) <920d3c18-5f3f-391c-b4c0-adcbc1d75395> /Applications/Max.app/Contents/Resources/C74/externals/max/js.mxo/Contents/MacOS/js
       0x15f460000 -        0x15f463fff libmozjs185.dylib (*) <784b3a9e-140d-3efa-9157-59705ccf1962> /Applications/Ableton Live 12 Suite.app/Contents/App-Resources/Max/Max.app/Contents/Frameworks/libmozjs185.dylib
       0x15f7b0000 -        0x15f7b7fff com.cycling74.jsjitter (9.1.2 (07b9c4cbe57)) <4c509340-e210-3a22-8bf9-f09aa5a6f6ce> /Applications/Max.app/Contents/Resources/C74/jsextensions/jitter/jsjitter.mxo/Contents/MacOS/jsjitter
       0x168534000 -        0x16853bfff com.cycling74.jsliveapi (9.1.2 (07b9c4cbe57)) <be0abb57-36fc-384a-9fc3-cc7de6bcf4d7> /Applications/Max.app/Contents/Resources/C74/jsextensions/m4l/jsliveapi.mxo/Contents/MacOS/jsliveapi
       0x16854c000 -        0x16854ffff com.cycling74.jsfolder (9.1.2 (07b9c4cbe57)) <9865d39a-3ad0-33a2-bdf9-2f5a6c046f72> /Applications/Max.app/Contents/Resources/C74/jsextensions/max/jsfolder.mxo/Contents/MacOS/jsfolder
       0x168560000 -        0x168563fff com.cycling74.jsmaxobjlistener (9.1.2 (07b9c4cbe57)) <2e6d261c-2fa3-301a-897d-1a19cb069d26> /Applications/Max.app/Contents/Resources/C74/jsextensions/max/jsmaxobjlistener.mxo/Contents/MacOS/jsmaxobjlistener
       0x168574000 -        0x168577fff com.cycling74.jsparaminfoprovider (9.1.2 (07b9c4cbe57)) <eea3bec0-5500-3811-b12d-b66c6ef5b1b3> /Applications/Max.app/Contents/Resources/C74/jsextensions/max/jsparaminfoprovider.mxo/Contents/MacOS/jsparaminfoprovider
       0x168588000 -        0x16858bfff com.cycling74.jsparamlistener (9.1.2 (07b9c4cbe57)) <c0658c58-a728-306d-810f-39b648c267d9> /Applications/Max.app/Contents/Resources/C74/jsextensions/max/jsparamlistener.mxo/Contents/MacOS/jsparamlistener
       0x168e70000 -        0x168e73fff com.cycling74.jsrequire (9.1.2 (07b9c4cbe57)) <ef8c7588-6cf2-3648-bd4d-f823f6bd54af> /Applications/Max.app/Contents/Resources/C74/jsextensions/max/jsrequire.mxo/Contents/MacOS/jsrequire
       0x168eb0000 -        0x168eb3fff com.cycling74.jsrx256 (9.1.2 (07b9c4cbe57)) <9eaed4e7-d429-32d0-b776-c564e888cc6e> /Applications/Max.app/Contents/Resources/C74/jsextensions/max/jsrx256.mxo/Contents/MacOS/jsrx256
       0x168ec4000 -        0x168ec7fff com.cycling74.jssnapshotapi (9.1.2 (07b9c4cbe57)) <dd65c8b2-4b8c-350f-b9ff-cc9c031e01a2> /Applications/Max.app/Contents/Resources/C74/jsextensions/max/jssnapshotapi.mxo/Contents/MacOS/jssnapshotapi
       0x168ed8000 -        0x168edbfff com.cycling74.jssqlite (9.1.2 (07b9c4cbe57)) <6a41a32c-69ba-3f16-abea-959f4ae5bc00> /Applications/Max.app/Contents/Resources/C74/jsextensions/max/jssqlite.mxo/Contents/MacOS/jssqlite
       0x168eec000 -        0x168eeffff com.cycling74.jssqlresult (9.1.2 (07b9c4cbe57)) <c7ec8205-8db1-3bbe-af3d-377cdb6324a6> /Applications/Max.app/Contents/Resources/C74/jsextensions/max/jssqlresult.mxo/Contents/MacOS/jssqlresult
       0x169000000 -        0x169003fff com.cycling74.jstrigger (9.1.2 (07b9c4cbe57)) <87b08e91-12ee-3be0-8a5c-af892e29f699> /Applications/Max.app/Contents/Resources/C74/jsextensions/max/jstrigger.mxo/Contents/MacOS/jstrigger
       0x169014000 -        0x169017fff com.cycling74.jsxmlhttprequest (9.1.2 (07b9c4cbe57)) <84d6da4a-19b1-3fd0-adc1-80b59e4634b4> /Applications/Max.app/Contents/Resources/C74/jsextensions/max/jsxmlhttprequest.mxo/Contents/MacOS/jsxmlhttprequest
       0x169028000 -        0x16902bfff com.cycling74.jsbuffer (9.1.2 (07b9c4cbe57)) <5112170c-05cf-3965-a0f0-48b8ca9dfda4> /Applications/Max.app/Contents/Resources/C74/jsextensions/msp/jsbuffer.mxo/Contents/MacOS/jsbuffer
       0x17ba30000 -        0x17ca57fff com.SmackTheDrums.SmackTheDrums (0.1.0) <485bcab4-0a64-388b-b8f9-b55051da0d44> /Users/USER/Library/Audio/Plug-Ins/VST3/SmackTheDrums.vst3/Contents/MacOS/SmackTheDrums
       0x12ae48000 -        0x12af83fff com.apple.audio.units.Components (1.14) <674f87bc-21a4-309b-9e43-c7ba1cd95d7b> /System/Library/Components/CoreAudio.component/Contents/MacOS/CoreAudio
       0x348000000 -        0x3488e3fff com.apple.audio.codecs.Components (7.0) <42d57795-e86d-36e5-9fae-f2ba92271fba> /System/Library/Components/AudioCodecs.component/Contents/MacOS/AudioCodecs
       0x34b018000 -        0x34bcb3fff com.gravel.possibilitydistortion (0.1.0) <cbce898f-88fb-3967-a815-1ede0010df08> /Library/Audio/Plug-Ins/VST3/Gravel Possibility Distortion.vst3/Contents/MacOS/Gravel Possibility Distortion
       0x18e501000 -        0x18f2ea2ff com.apple.Foundation (6.9) <e8f6a451-0acc-3e05-b18f-fec6618ce44a> /System/Library/Frameworks/Foundation.framework/Versions/C/Foundation
       0x18cf11000 -        0x18d44ffff com.apple.CoreFoundation (6.9) <df489a59-b4f6-32b8-9bb4-9b832960aa52> /System/Library/Frameworks/CoreFoundation.framework/Versions/A/CoreFoundation
       0x19895d000 -        0x198c63fdf com.apple.HIToolbox (2.1.1) <9286e29f-fcee-31d0-acea-2842ea23bedf> /System/Library/Frameworks/Carbon.framework/Versions/A/Frameworks/HIToolbox.framework/Versions/A/HIToolbox
       0x190e79000 -        0x19230ac7f com.apple.AppKit (6.9) <5d0da1bd-412c-3ed8-84e9-40ca62fe7b42> /System/Library/Frameworks/AppKit.framework/Versions/C/AppKit
       0x18cafc000 -        0x18cb974cf dyld (*) <9cf0401a-a938-389e-a77d-9e9608076ccf> /usr/lib/dyld
               0x0 - 0xffffffffffffffff ??? (*) <00000000-0000-0000-0000-000000000000> ???
       0x18ce60000 -        0x18ce9b653 libsystem_kernel.dylib (*) <60485b6f-67e5-38c1-aec9-efd6031ff166> /usr/lib/system/libsystem_kernel.dylib
       0x18ce9c000 -        0x18cea8a47 libsystem_pthread.dylib (*) <647b91fc-96d3-3bbb-af08-970df45257c8> /usr/lib/system/libsystem_pthread.dylib
       0x1a94df000 -        0x1a9599d1f com.apple.audio.midi.CoreMIDI (2.0) <cf441ec3-9851-3874-8fe1-17d470031ea5> /System/Library/Frameworks/CoreMIDI.framework/Versions/A/CoreMIDI
       0x198506000 -        0x19852dddf com.apple.audio.caulk (1.0) <5ce1b98f-c512-379d-9a42-5cce5923bf9d> /System/Library/PrivateFrameworks/caulk.framework/Versions/A/caulk
       0x18cdb5000 -        0x18ce41ff7 libc++.1.dylib (*) <875203a1-087b-33a6-93a5-928bb7e9114c> /usr/lib/libc++.1.dylib
       0x18fb9e000 -        0x1902d225f com.apple.audio.CoreAudio (5.0) <b2b97b04-6275-3031-8a89-be6105f1e8f3> /System/Library/Frameworks/CoreAudio.framework/Versions/A/CoreAudio
       0x196834000 -        0x196894ddf com.apple.CoreVideo (1.8) <54956113-d920-3d30-b023-f4f1419e7441> /System/Library/Frameworks/CoreVideo.framework/Versions/A/CoreVideo

External Modification Summary:
  Calls made by other processes targeting this process:
    task_for_pid: 0
    thread_create: 0
    thread_set_state: 0
  Calls made by this process:
    task_for_pid: 0
    thread_create: 0
    thread_set_state: 0
  Calls made by all processes on this machine:
    task_for_pid: 0
    thread_create: 0
    thread_set_state: 0


-----------
Full Report
-----------

{"app_name":"Live","timestamp":"2026-07-20 03:03:59.00 +0200","app_version":"12.3.5 (2026-01-14_c4ac4719dc)","slice_uuid":"22acf5c7-4011-382b-a5d0-b782292a7879","build_version":"12.3.5 (2026-01-14_c4ac4719dc)","platform":1,"bundleID":"com.ableton.live","share_with_app_devs":0,"is_first_party":0,"bug_type":"309","os_version":"macOS 15.5 (24F74)","roots_installed":0,"name":"Live","incident_id":"05AF6BA8-51B9-43CC-9FDA-4DA29AAD67B0"}
{
  "uptime" : 410000,
  "procRole" : "Foreground",
  "version" : 2,
  "userID" : 501,
  "deployVersion" : 210,
  "modelCode" : "Mac14,7",
  "coalitionID" : 609,
  "osVersion" : {
    "train" : "macOS 15.5",
    "build" : "24F74",
    "releaseType" : "User"
  },
  "captureTime" : "2026-07-20 03:03:09.7723 +0200",
  "codeSigningMonitor" : 1,
  "incident" : "05AF6BA8-51B9-43CC-9FDA-4DA29AAD67B0",
  "pid" : 99818,
  "translated" : false,
  "cpuType" : "ARM-64",
  "roots_installed" : 0,
  "bug_type" : "309",
  "procLaunch" : "2026-07-09 13:56:30.0973 +0200",
  "procStartAbsTime" : 5596751993693,
  "procExitAbsTime" : 10044711035916,
  "procName" : "Live",
  "procPath" : "\/Applications\/Ableton Live 12 Suite.app\/Contents\/MacOS\/Live",
  "bundleInfo" : {"CFBundleShortVersionString":"12.3.5 (2026-01-14_c4ac4719dc)","CFBundleVersion":"12.3.5 (2026-01-14_c4ac4719dc)","CFBundleIdentifier":"com.ableton.live"},
  "storeInfo" : {"deviceIdentifierForVendor":"B714374A-B4E9-5647-AD71-05969D11B6E1","thirdParty":true},
  "parentProc" : "launchd",
  "parentPid" : 1,
  "coalitionName" : "com.ableton.live",
  "crashReporterKey" : "E406B683-B03C-1202-28CB-EADD5B7C065D",
  "appleIntelligenceStatus" : {"reasons":["selectedLanguageIneligible","selectedLanguageDoesNotMatchSelectedSiriLanguage","selectedSiriLanguageIneligible","countryBillingIneligible"],"state":"unavailable"},
  "codeSigningID" : "com.ableton.live",
  "codeSigningTeamID" : "",
  "codeSigningFlags" : 570425857,
  "codeSigningValidationCategory" : 10,
  "codeSigningTrustLevel" : 4294967295,
  "codeSigningAuxiliaryInfo" : 0,
  "instructionByteStream" : {"beforePC":"\/wMBkcADX9b\/QwDR4A8AueAPQLn\/QwCRwANf1v9DANHgBwD56AdA+Q==","atPC":"AAlAuf9DAJHAA1\/W\/0MA0eAHAPnoB0D5AA1Auf9DAJHAA1\/W\/0MA0Q=="},
  "bootSessionUUID" : "356626FD-17A7-4C97-A631-F14DEFACDFEC",
  "wakeTime" : 13604,
  "sleepWakeUUID" : "AEE66E85-840B-40EE-8FF8-04AB92EBF56C",
  "sip" : "enabled",
  "vmRegionInfo" : "0x40 is not in any region.  Bytes before following region: 4339531712\n      REGION TYPE                    START - END         [ VSIZE] PRT\/MAX SHRMOD  REGION DETAIL\n      UNUSED SPACE AT START\n--->  \n      __TEXT                      102a80000-108cac000    [ 98.2M] r-x\/r-x SM=COW  \/Applications\/Ableton Live 12 Suite.app\/Contents\/MacOS\/Live",
  "exception" : {"codes":"0x0000000000000001, 0x0000000000000040","rawCodes":[1,64],"type":"EXC_BAD_ACCESS","signal":"SIGSEGV","subtype":"KERN_INVALID_ADDRESS at 0x0000000000000040"},
  "termination" : {"flags":0,"code":11,"namespace":"SIGNAL","indicator":"Segmentation fault: 11","byProc":"exc handler","byPid":99818},
  "vmregioninfo" : "0x40 is not in any region.  Bytes before following region: 4339531712\n      REGION TYPE                    START - END         [ VSIZE] PRT\/MAX SHRMOD  REGION DETAIL\n      UNUSED SPACE AT START\n--->  \n      __TEXT                      102a80000-108cac000    [ 98.2M] r-x\/r-x SM=COW  \/Applications\/Ableton Live 12 Suite.app\/Contents\/MacOS\/Live",
  "extMods" : {"caller":{"thread_create":0,"thread_set_state":0,"task_for_pid":0},"system":{"thread_create":0,"thread_set_state":0,"task_for_pid":0},"targeted":{"thread_create":0,"thread_set_state":0,"task_for_pid":0},"warnings":0},
  "faultingThread" : 0,
  "threads" : [{"threadState":{"x":[{"value":56},{"value":170},{"value":80},{"value":131},{"value":348},{"value":1099511628288},{"value":0},{"value":105553134276660},{"value":56},{"value":0},{"value":8505728768,"symbolLocation":0,"symbol":"_main_thread"},{"value":1},{"value":4294967293},{"value":0},{"value":0},{"value":0},{"value":8505728768,"symbolLocation":0,"symbol":"_main_thread"},{"value":8505728768,"symbolLocation":0,"symbol":"_main_thread"},{"value":0},{"value":14143905608,"sourceFile":"juce_audio_plugin_client_VST3.cpp","symbol":"non-virtual thunk to juce::JuceVST3EditController::createView(char const*)","symbolLocation":0},{"value":4433245738},{"value":105553159416208},{"value":4446480072,"symbolLocation":5624,"symbol":"ableton::live_library::kVirtualPathSep<char16_t>"},{"value":1},{"value":4453145152},{"value":4453145152},{"value":554828005},{"value":10044706780210},{"value":6683179016,"symbolLocation":0,"symbol":"_timerRelease"}],"flavor":"ARM_THREAD_STATE64","lr":{"value":14144237400},"cpsr":{"value":1610616832},"fp":{"value":6127342192},"sp":{"value":6127342160},"esr":{"value":2449473542,"description":"(Data Abort) byte read Translation fault"},"pc":{"value":14143979960,"matchesCrashFrame":1},"far":{"value":64}},"id":3333339,"triggered":true,"name":"MainThread","queue":"com.apple.main-thread","frames":[{"imageOffset":688568,"sourceLine":140,"sourceFile":"juce_Rectangle.h","symbol":"juce::Rectangle<int>::getWidth() const","imageIndex":75,"symbolLocation":12},{"imageOffset":946008,"sourceLine":280,"sourceFile":"juce_Component.h","symbol":"juce::Component::getWidth() const","imageIndex":75,"symbolLocation":28},{"imageOffset":2121976,"sourceLine":848,"sourceFile":"juce_Component.cpp","symbol":"juce::Component::setBounds(int, int, int, int)","imageIndex":75,"symbolLocation":192},{"imageOffset":2018620,"sourceLine":961,"sourceFile":"juce_Component.cpp","symbol":"juce::Component::setBounds(juce::Rectangle<int>)","imageIndex":75,"symbolLocation":120},{"imageOffset":946328,"sourceLine":135,"sourceFile":"PluginEditor.cpp","symbol":"GravelPossibilityDistortionAudioProcessorEditor::resized()","imageIndex":75,"symbolLocation":308},{"imageOffset":2122936,"sourceLine":922,"sourceFile":"juce_Component.cpp","symbol":"juce::Component::sendMovedResizedMessages(bool, bool)","imageIndex":75,"symbolLocation":156},{"imageOffset":2122764,"sourceLine":904,"sourceFile":"juce_Component.cpp","symbol":"juce::Component::sendMovedResizedMessagesIfPending()","imageIndex":75,"symbolLocation":148},{"imageOffset":2122600,"sourceLine":890,"sourceFile":"juce_Component.cpp","symbol":"juce::Component::setBounds(int, int, int, int)","imageIndex":75,"symbolLocation":816},{"imageOffset":2072460,"sourceLine":954,"sourceFile":"juce_Component.cpp","symbol":"juce::Component::setSize(int, int)","imageIndex":75,"symbolLocation":76},{"imageOffset":941172,"sourceLine":74,"sourceFile":"PluginEditor.cpp","symbol":"GravelPossibilityDistortionAudioProcessorEditor::GravelPossibilityDistortionAudioProcessorEditor(GravelPossibilityDistortionAudioProcessor&)","imageIndex":75,"symbolLocation":356},{"imageOffset":944684,"sourceLine":73,"sourceFile":"PluginEditor.cpp","symbol":"GravelPossibilityDistortionAudioProcessorEditor::GravelPossibilityDistortionAudioProcessorEditor(GravelPossibilityDistortionAudioProcessor&)","imageIndex":75,"symbolLocation":36},{"imageOffset":911188,"sourceLine":110,"sourceFile":"PluginProcessor.cpp","symbol":"GravelPossibilityDistortionAudioProcessor::createEditor()","imageIndex":75,"symbolLocation":44},{"imageOffset":11083536,"sourceLine":919,"sourceFile":"juce_AudioProcessor.cpp","symbol":"juce::AudioProcessor::createEditorIfNeeded()","imageIndex":75,"symbolLocation":240},{"imageOffset":651868,"sourceLine":2336,"sourceFile":"juce_audio_plugin_client_VST3.cpp","symbol":"juce::JuceVST3EditController::JuceVST3Editor::ContentWrapperComponent::createEditor(juce::AudioProcessor&)","imageIndex":75,"symbolLocation":36},{"imageOffset":644896,"sourceLine":2516,"sourceFile":"juce_audio_plugin_client_VST3.cpp","symbol":"juce::JuceVST3EditController::JuceVST3Editor::createContentWrapperComponentIfNeeded()","imageIndex":75,"symbolLocation":116},{"imageOffset":644192,"sourceLine":1939,"sourceFile":"juce_audio_plugin_client_VST3.cpp","symbol":"juce::JuceVST3EditController::JuceVST3Editor::JuceVST3Editor(juce::JuceVST3EditController&, juce::JuceAudioProcessor&)","imageIndex":75,"symbolLocation":240},{"imageOffset":643936,"sourceLine":1938,"sourceFile":"juce_audio_plugin_client_VST3.cpp","symbol":"juce::JuceVST3EditController::JuceVST3Editor::JuceVST3Editor(juce::JuceVST3EditController&, juce::JuceAudioProcessor&)","imageIndex":75,"symbolLocation":44},{"imageOffset":610032,"sourceLine":1453,"sourceFile":"juce_audio_plugin_client_VST3.cpp","symbol":"juce::JuceVST3EditController::createView(char const*)","imageIndex":75,"symbolLocation":276},{"imageOffset":50734264,"imageIndex":0},{"imageOffset":50675592,"imageIndex":0},{"imageOffset":50674972,"imageIndex":0},{"imageOffset":50674656,"imageIndex":0},{"imageOffset":50713728,"imageIndex":0},{"imageOffset":50199596,"imageIndex":0},{"imageOffset":50199388,"imageIndex":0},{"imageOffset":50199724,"imageIndex":0},{"imageOffset":21816976,"imageIndex":0},{"imageOffset":21858028,"imageIndex":0},{"imageOffset":50199092,"imageIndex":0},{"imageOffset":50275348,"imageIndex":0},{"imageOffset":23878764,"imageIndex":0},{"imageOffset":23879064,"symbol":"void ableton::utility::detail::CallbackTypes<TOneShotTimer, void>::CallMemberFunc<&TOneShotTimer::OnTimer()>(void*)","symbolLocation":80,"imageIndex":0},{"imageOffset":20701024,"imageIndex":0},{"imageOffset":20700436,"imageIndex":0},{"imageOffset":23274936,"imageIndex":0},{"imageOffset":606156,"symbol":"__NSFireTimer","symbolLocation":104,"imageIndex":76},{"imageOffset":613908,"symbol":"__CFRUNLOOP_IS_CALLING_OUT_TO_A_TIMER_CALLBACK_FUNCTION__","symbolLocation":32,"imageIndex":77},{"imageOffset":613076,"symbol":"__CFRunLoopDoTimer","symbolLocation":980,"imageIndex":77},{"imageOffset":611856,"symbol":"__CFRunLoopDoTimers","symbolLocation":332,"imageIndex":77},{"imageOffset":506392,"symbol":"__CFRunLoopRun","symbolLocation":1848,"imageIndex":77},{"imageOffset":502872,"symbol":"CFRunLoopRunSpecific","symbolLocation":572,"imageIndex":77},{"imageOffset":799356,"symbol":"RunCurrentEventLoopInMode","symbolLocation":324,"imageIndex":78},{"imageOffset":812264,"symbol":"ReceiveNextEventCommon","symbolLocation":676,"imageIndex":78},{"imageOffset":2430084,"symbol":"_BlockUntilNextEventMatchingListInModeWithFilter","symbolLocation":76,"imageIndex":78},{"imageOffset":240308,"symbol":"_DPSNextEvent","symbolLocation":684,"imageIndex":79},{"imageOffset":10327472,"symbol":"-[NSApplication(NSEventRouting) _nextEventMatchingEventMask:untilDate:inMode:dequeue:]","symbolLocation":688,"imageIndex":79},{"imageOffset":187492,"symbol":"-[NSApplication run]","symbolLocation":480,"imageIndex":79},{"imageOffset":23274044,"imageIndex":0},{"imageOffset":27544,"symbol":"start","symbolLocation":6076,"imageIndex":80}]},{"id":3333510,"name":"NetIoThread","threadState":{"x":[{"value":14},{"value":4357607236},{"value":0},{"value":6130774108},{"value":6130773752},{"value":11},{"value":0},{"value":0},{"value":1},{"value":2},{"value":0},{"value":2},{"value":0},{"value":2},{"value":0},{"value":0},{"value":18446744073709551580},{"value":8522843408},{"value":0},{"value":105553159752144},{"value":11},{"value":4432939084},{"value":6130773832},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":4357607272},"cpsr":{"value":1610616832},"fp":{"value":6130773776},"sp":{"value":6130773760},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658853808},"far":{"value":0}},"frames":[{"imageOffset":2992,"symbol":"semaphore_wait_trap","symbolLocation":8,"imageIndex":82},{"imageOffset":18075496,"imageIndex":0},{"imageOffset":22152356,"symbol":"void ableton::utility::detail::CallbackTypes<ANetIoThread, void, TUserData>::CallMemberFunc<&ANetIoThread::Main(TUserData)>(void*, TUserData)","symbolLocation":100,"imageIndex":0},{"imageOffset":18173204,"imageIndex":0},{"imageOffset":18176564,"imageIndex":0},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333513,"frames":[{"imageOffset":3124,"symbol":"mach_msg2_trap","symbolLocation":8,"imageIndex":82},{"imageOffset":78752,"symbol":"mach_msg2_internal","symbolLocation":76,"imageIndex":82},{"imageOffset":38756,"symbol":"mach_msg_overwrite","symbolLocation":484,"imageIndex":82},{"imageOffset":4008,"symbol":"mach_msg","symbolLocation":24,"imageIndex":82},{"imageOffset":257708,"symbol":"XServerMachPort::ReceiveMessage(int&, void*, int&)","symbolLocation":104,"imageIndex":84},{"imageOffset":395072,"symbol":"MIDIProcess::MIDIInPortThread::Run()","symbolLocation":148,"imageIndex":84},{"imageOffset":350060,"symbol":"CADeprecated::XThread::RunHelper(void*)","symbolLocation":48,"imageIndex":84},{"imageOffset":392772,"symbol":"CADeprecated::CAPThread::Entry(CADeprecated::CAPThread*)","symbolLocation":96,"imageIndex":84},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}],"threadState":{"x":[{"value":268451845},{"value":17179869186},{"value":0},{"value":106665512796160},{"value":0},{"value":106665512796160},{"value":100},{"value":0},{"value":0},{"value":17179869184},{"value":100},{"value":0},{"value":0},{"value":0},{"value":24835},{"value":2168752463488745472},{"value":18446744073709551569},{"value":8522835640},{"value":0},{"value":0},{"value":100},{"value":106665512796160},{"value":0},{"value":106665512796160},{"value":6131345876},{"value":0},{"value":17179869186},{"value":18446744073709550527},{"value":2}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6658929568},"cpsr":{"value":4096},"fp":{"value":6131345536},"sp":{"value":6131345456},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658853940},"far":{"value":0}}},{"id":3333549,"name":"FileThread","threadState":{"x":[{"value":14},{"value":4357607236},{"value":131584},{"value":28},{"value":4024},{"value":484},{"value":12072},{"value":2320},{"value":1},{"value":5052039168},{"value":72},{"value":2},{"value":48},{"value":64},{"value":0},{"value":2869},{"value":18446744073709551580},{"value":2816},{"value":0},{"value":105553159772480},{"value":4453599886},{"value":4453597184},{"value":6135934792},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":4357607272},"cpsr":{"value":1610616832},"fp":{"value":6135934704},"sp":{"value":6135934688},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658853808},"far":{"value":0}},"frames":[{"imageOffset":2992,"symbol":"semaphore_wait_trap","symbolLocation":8,"imageIndex":82},{"imageOffset":18075496,"imageIndex":0},{"imageOffset":34634384,"imageIndex":0},{"imageOffset":18173204,"imageIndex":0},{"imageOffset":18176564,"imageIndex":0},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333555,"name":"IPC Channel Reader","threadState":{"x":[{"value":4},{"value":0},{"value":500},{"value":6136507312},{"value":6136507296},{"value":2810183925},{"value":0},{"value":0},{"value":1},{"value":105553151417992},{"value":0},{"value":2},{"value":0},{"value":0},{"value":2095104},{"value":2043},{"value":230},{"value":244},{"value":0},{"value":105553151417984},{"value":105553159953120},{"value":500},{"value":2},{"value":2},{"value":105553159953140},{"value":4},{"value":1},{"value":0},{"value":105553159953136}],"flavor":"ARM_THREAD_STATE64","lr":{"value":4406564328},"cpsr":{"value":1610616832},"fp":{"value":6136507392},"sp":{"value":6136507296},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658888856},"far":{"value":0}},"frames":[{"imageOffset":38040,"symbol":"poll","symbolLocation":8,"imageIndex":82},{"imageOffset":67032552,"imageIndex":0},{"imageOffset":22326732,"imageIndex":0},{"imageOffset":22327932,"imageIndex":0},{"imageOffset":22333756,"imageIndex":0},{"imageOffset":22332656,"imageIndex":0},{"imageOffset":18173204,"imageIndex":0},{"imageOffset":18176564,"imageIndex":0},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333556,"name":"caulk.messenger.shared:17","threadState":{"x":[{"value":14},{"value":1},{"value":0},{"value":1},{"value":0},{"value":1},{"value":0},{"value":0},{"value":0},{"value":4294967295},{"value":0},{"value":0},{"value":4568973336},{"value":6325677688},{"value":7},{"value":0},{"value":18446744073709551580},{"value":8522838072},{"value":0},{"value":105553133385984},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6850378952},"cpsr":{"value":2147487744},"fp":{"value":6325677952},"sp":{"value":6325677920},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658853808},"far":{"value":0}},"frames":[{"imageOffset":2992,"symbol":"semaphore_wait_trap","symbolLocation":8,"imageIndex":82},{"imageOffset":7368,"symbol":"caulk::semaphore::timed_wait(double)","symbolLocation":224,"imageIndex":85},{"imageOffset":7024,"symbol":"caulk::concurrent::details::worker_thread::run()","symbolLocation":32,"imageIndex":85},{"imageOffset":6212,"symbol":"void* caulk::thread_proxy<std::__1::tuple<caulk::thread::attributes, void (caulk::concurrent::details::worker_thread::*)(), std::__1::tuple<caulk::concurrent::details::worker_thread*>>>(void*)","symbolLocation":96,"imageIndex":85},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333557,"name":"caulk.messenger.shared:high","threadState":{"x":[{"value":14},{"value":58627},{"value":58627},{"value":49},{"value":4294967295},{"value":0},{"value":0},{"value":0},{"value":0},{"value":4294967295},{"value":1},{"value":105553154072744},{"value":0},{"value":0},{"value":0},{"value":0},{"value":18446744073709551580},{"value":8522838072},{"value":0},{"value":105553133386400},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6850378952},"cpsr":{"value":2147487744},"fp":{"value":6326251392},"sp":{"value":6326251360},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658853808},"far":{"value":0}},"frames":[{"imageOffset":2992,"symbol":"semaphore_wait_trap","symbolLocation":8,"imageIndex":82},{"imageOffset":7368,"symbol":"caulk::semaphore::timed_wait(double)","symbolLocation":224,"imageIndex":85},{"imageOffset":7024,"symbol":"caulk::concurrent::details::worker_thread::run()","symbolLocation":32,"imageIndex":85},{"imageOffset":6212,"symbol":"void* caulk::thread_proxy<std::__1::tuple<caulk::thread::attributes, void (caulk::concurrent::details::worker_thread::*)(), std::__1::tuple<caulk::concurrent::details::worker_thread*>>>(void*)","symbolLocation":96,"imageIndex":85},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333648,"frames":[{"imageOffset":27908,"symbol":"kevent","symbolLocation":8,"imageIndex":82},{"imageOffset":67122136,"imageIndex":0},{"imageOffset":67022924,"imageIndex":0},{"imageOffset":67120720,"imageIndex":0},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}],"threadState":{"x":[{"value":4},{"value":0},{"value":0},{"value":4454097228},{"value":32},{"value":0},{"value":35295918502798906},{"value":0},{"value":6326824704},{"value":18446744073708551616},{"value":1000000},{"value":1},{"value":1},{"value":30786325584896},{"value":4454098252},{"value":2043},{"value":363},{"value":29},{"value":0},{"value":4454097216},{"value":0},{"value":4454098252},{"value":4454097200},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":4406653912},"cpsr":{"value":2684358656},"fp":{"value":6326824752},"sp":{"value":6326824672},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658878724},"far":{"value":0}}},{"id":3333660,"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123544,"symbol":"std::__1::condition_variable::wait(std::__1::unique_lock<std::__1::mutex>&)","symbolLocation":32,"imageIndex":86},{"imageOffset":52089220,"imageIndex":0},{"imageOffset":82933004,"imageIndex":0},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}],"threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":0},{"value":6327398024},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":4806620024},{"value":4806620136},{"value":6327398624},{"value":0},{"value":0},{"value":0},{"value":1},{"value":256},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6327398144},"sp":{"value":6327398000},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}}},{"id":3333661,"name":"browser","threadState":{"x":[{"value":14},{"value":5829504752},{"value":73896},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":5290464176},{"value":2},{"value":0},{"value":2},{"value":0},{"value":0},{"value":1987326464},{"value":0},{"value":18446744073709551580},{"value":8522843408},{"value":0},{"value":5290464416},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":4406541488},"cpsr":{"value":1610616832},"fp":{"value":6327971616},"sp":{"value":6327971600},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658853808},"far":{"value":0}},"frames":[{"imageOffset":2992,"symbol":"semaphore_wait_trap","symbolLocation":8,"imageIndex":82},{"imageOffset":67009712,"imageIndex":0},{"imageOffset":63706656,"symbol":"ableton::live_library::AsyncLibraryHub::Impl::workerThreadMain(ableton::live_library::AsyncLibraryHub::Impl*)","symbolLocation":380,"imageIndex":0},{"imageOffset":63708732,"imageIndex":0},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333662,"frames":[{"imageOffset":27908,"symbol":"kevent","symbolLocation":8,"imageIndex":82},{"imageOffset":14460724,"imageIndex":0},{"imageOffset":14465172,"imageIndex":0},{"imageOffset":14464376,"imageIndex":0},{"imageOffset":14489968,"imageIndex":0},{"imageOffset":14489788,"imageIndex":0},{"imageOffset":14489596,"imageIndex":0},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}],"threadState":{"x":[{"value":4},{"value":0},{"value":0},{"value":6328539344},{"value":128},{"value":6328539312},{"value":105553116502480},{"value":105553148002552},{"value":5290485696},{"value":1099511628034},{"value":1099511628032},{"value":1099511628034},{"value":24000000},{"value":183927},{"value":1784509389687548},{"value":3582052412},{"value":363},{"value":8522833464},{"value":0},{"value":5290485600},{"value":6328544816},{"value":0},{"value":0},{"value":5290471312},{"value":5290471312},{"value":1},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":4353992500},"cpsr":{"value":1610616832},"fp":{"value":6328544560},"sp":{"value":6328539248},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658878724},"far":{"value":0}}},{"id":3333779,"name":"JUCE v8.0.12: Timer","threadState":{"x":[{"value":260},{"value":0},{"value":93054720},{"value":0},{"value":0},{"value":160},{"value":0},{"value":299999709},{"value":6134786744},{"value":0},{"value":209664},{"value":900500023358210},{"value":900500023358210},{"value":209664},{"value":0},{"value":900500023358208},{"value":305},{"value":8522833760},{"value":0},{"value":5555822112},{"value":5555822176},{"value":6134788320},{"value":299999709},{"value":0},{"value":93054720},{"value":93054721},{"value":93054976},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6134786864},"sp":{"value":6134786720},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":9832004,"symbol":"std::__1::cv_status std::__1::condition_variable::wait_for<long long, std::__1::ratio<1l, 1000000000l>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>> const&)","symbolLocation":388,"imageIndex":7},{"imageOffset":9831052,"symbol":"void std::__1::condition_variable::__do_timed_wait[abi:ne190102]<std::__1::chrono::steady_clock>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":7},{"imageOffset":9970096,"symbol":"std::__1::cv_status std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&)","symbolLocation":160,"imageIndex":7},{"imageOffset":9969728,"symbol":"bool std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&, juce::WaitableEvent::wait(double) const::$_1)","symbolLocation":72,"imageIndex":7},{"imageOffset":9579160,"symbol":"bool std::__1::condition_variable::wait_for[abi:ne190102]<double, std::__1::ratio<1l, 1000l>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000l>> const&, juce::WaitableEvent::wait(double) const::$_1)","symbolLocation":100,"imageIndex":7},{"imageOffset":9388848,"symbol":"juce::WaitableEvent::wait(double) const","symbolLocation":188,"imageIndex":7},{"imageOffset":9004132,"symbol":"juce::Timer::TimerThread::run()","symbolLocation":340,"imageIndex":7},{"imageOffset":9391736,"symbol":"juce::Thread::threadEntryPoint()","symbolLocation":244,"imageIndex":7},{"imageOffset":9392492,"symbol":"juce::juce_threadEntryPoint(void*)","symbolLocation":24,"imageIndex":7},{"imageOffset":9767244,"symbol":"juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::operator()(void*) const","symbolLocation":148,"imageIndex":7},{"imageOffset":9767084,"symbol":"juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::__invoke(void*)","symbolLocation":28,"imageIndex":7},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333861,"name":"sentry-http","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":65704},{"value":0},{"value":999998000},{"value":6420016840},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":105553206080528},{"value":105553206080432},{"value":6420017376},{"value":999998000},{"value":0},{"value":0},{"value":47128833},{"value":47129088},{"value":1},{"value":18446744073709551615}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6420016960},"sp":{"value":6420016816},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":8762788,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333862,"name":"JUCE Timer","threadState":{"x":[{"value":260},{"value":0},{"value":2309204736},{"value":0},{"value":0},{"value":160},{"value":0},{"value":299998834},{"value":6420590024},{"value":0},{"value":2896128},{"value":12438775047926018},{"value":12438775047926018},{"value":2896128},{"value":0},{"value":12438775047926016},{"value":305},{"value":8522833760},{"value":0},{"value":5618046416},{"value":5618046480},{"value":6420590816},{"value":299998834},{"value":0},{"value":2309204736},{"value":2309204737},{"value":2309204992},{"value":5634953216},{"value":4294967295}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6420590144},"sp":{"value":6420590000},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":4284628,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333863,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499998792},{"value":6421163512},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5355640888},{"value":5355640952},{"value":6421164256},{"value":499998792},{"value":0},{"value":0},{"value":93656321},{"value":93656576},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6421163632},"sp":{"value":6421163488},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333864,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499998834},{"value":6421736952},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5618046840},{"value":5618046904},{"value":6421737696},{"value":499998834},{"value":0},{"value":0},{"value":93655553},{"value":93655808},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6421737072},"sp":{"value":6421736928},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333865,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499999958},{"value":6422310392},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5618047320},{"value":5618047384},{"value":6422311136},{"value":499999958},{"value":0},{"value":0},{"value":93656577},{"value":93656832},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6422310512},"sp":{"value":6422310368},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333866,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499999958},{"value":6422883832},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5618047800},{"value":5618047864},{"value":6422884576},{"value":499999958},{"value":0},{"value":0},{"value":93656321},{"value":93656576},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6422883952},"sp":{"value":6422883808},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333867,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499999959},{"value":6423457272},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5618048280},{"value":5618048344},{"value":6423458016},{"value":499999959},{"value":0},{"value":0},{"value":93657089},{"value":93657344},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6423457392},"sp":{"value":6423457248},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333868,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499998709},{"value":6424030712},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5618048760},{"value":5618048824},{"value":6424031456},{"value":499998709},{"value":0},{"value":0},{"value":93656577},{"value":93656832},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6424030832},"sp":{"value":6424030688},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333869,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499998917},{"value":6424604152},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5618049240},{"value":5618049304},{"value":6424604896},{"value":499998917},{"value":0},{"value":0},{"value":93657089},{"value":93657344},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6424604272},"sp":{"value":6424604128},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333870,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499999875},{"value":6425177592},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5618049720},{"value":5618049784},{"value":6425178336},{"value":499999875},{"value":0},{"value":0},{"value":93656833},{"value":93657088},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6425177712},"sp":{"value":6425177568},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333871,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499999958},{"value":6425751032},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5289416024},{"value":5289416088},{"value":6425751776},{"value":499999958},{"value":0},{"value":0},{"value":93657601},{"value":93657856},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6425751152},"sp":{"value":6425751008},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333872,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499998875},{"value":6426324472},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5289416504},{"value":5289416568},{"value":6426325216},{"value":499998875},{"value":0},{"value":0},{"value":93655553},{"value":93655808},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6426324592},"sp":{"value":6426324448},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333873,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499998875},{"value":6426897912},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5289416984},{"value":5289417048},{"value":6426898656},{"value":499998875},{"value":0},{"value":0},{"value":93657857},{"value":93658112},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6426898032},"sp":{"value":6426897888},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333874,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499999958},{"value":6427471352},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5289417464},{"value":5289417528},{"value":6427472096},{"value":499999958},{"value":0},{"value":0},{"value":93656577},{"value":93656832},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6427471472},"sp":{"value":6427471328},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333875,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499998833},{"value":6428044792},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5289417944},{"value":5289418008},{"value":6428045536},{"value":499998833},{"value":0},{"value":0},{"value":93657345},{"value":93657600},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6428044912},"sp":{"value":6428044768},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333876,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499999958},{"value":6428618232},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5289418424},{"value":5289418488},{"value":6428618976},{"value":499999958},{"value":0},{"value":0},{"value":93656577},{"value":93656832},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6428618352},"sp":{"value":6428618208},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333877,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499999917},{"value":6429191672},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5289418904},{"value":5289418968},{"value":6429192416},{"value":499999917},{"value":0},{"value":0},{"value":93656321},{"value":93656576},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6429191792},"sp":{"value":6429191648},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333878,"name":"Pool","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":499998958},{"value":6429765112},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5289419384},{"value":5289419448},{"value":6429765856},{"value":499998958},{"value":0},{"value":0},{"value":93657345},{"value":93657600},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6429765232},"sp":{"value":6429765088},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":1646520,"imageIndex":10},{"imageOffset":1137456,"imageIndex":10},{"imageOffset":1462580,"imageIndex":10},{"imageOffset":1140676,"imageIndex":10},{"imageOffset":1605884,"imageIndex":10},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333892,"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123544,"symbol":"std::__1::condition_variable::wait(std::__1::unique_lock<std::__1::mutex>&)","symbolLocation":32,"imageIndex":86},{"imageOffset":9076344,"symbol":"spdlog::details::mpmc_blocking_queue<spdlog::details::async_msg>::dequeue(spdlog::details::async_msg&)","symbolLocation":72,"imageIndex":12},{"imageOffset":9076068,"symbol":"spdlog::details::thread_pool::process_next_msg_()","symbolLocation":124,"imageIndex":12},{"imageOffset":9075576,"symbol":"void* std::__1::__thread_proxy[abi:v160006]<std::__1::tuple<std::__1::unique_ptr<std::__1::__thread_struct, std::__1::default_delete<std::__1::__thread_struct>>, spdlog::details::thread_pool::thread_pool(unsigned long, unsigned long, std::__1::function<void ()>, std::__1::function<void ()>)::'lambda'()>>(void*)","symbolLocation":76,"imageIndex":12},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}],"threadState":{"x":[{"value":260},{"value":0},{"value":48384},{"value":0},{"value":0},{"value":160},{"value":0},{"value":0},{"value":6430338296},{"value":0},{"value":768},{"value":3298534884098},{"value":3298534884098},{"value":768},{"value":0},{"value":3298534884096},{"value":305},{"value":8522833760},{"value":0},{"value":5023413080},{"value":5023413144},{"value":6430339296},{"value":0},{"value":0},{"value":48384},{"value":48385},{"value":48640},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6430338416},"sp":{"value":6430338272},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}}},{"id":3333899,"name":"timer","threadState":{"x":[{"value":260},{"value":0},{"value":256},{"value":0},{"value":0},{"value":160},{"value":4},{"value":999899791},{"value":6430912008},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":105553131138616},{"value":105553131138512},{"value":6430912736},{"value":999899791},{"value":4},{"value":256},{"value":9413377},{"value":9413632},{"value":13682974720,"symbolLocation":8224,"symbol":"_st"},{"value":4591870180066957722}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6430912128},"sp":{"value":6430911984},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":3981484,"symbol":"std::__1::cv_status std::__1::condition_variable_any::wait_until<(anonymous namespace)::SysthreadMutexAdapter, std::__1::chrono::steady_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>((anonymous namespace)::SysthreadMutexAdapter&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>> const&)","symbolLocation":276,"imageIndex":12},{"imageOffset":3980676,"symbol":"(anonymous namespace)::Systimer::threadfun()","symbolLocation":256,"imageIndex":12},{"imageOffset":3981856,"symbol":"void* std::__1::__thread_proxy[abi:v160006]<std::__1::tuple<std::__1::unique_ptr<std::__1::__thread_struct, std::__1::default_delete<std::__1::__thread_struct>>, void ((anonymous namespace)::Systimer::*)(), (anonymous namespace)::Systimer*>>(void*)","symbolLocation":72,"imageIndex":12},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333978,"name":"com.apple.NSEventThread","threadState":{"x":[{"value":268451845},{"value":21592279046},{"value":8589934592,"symbolLocation":192,"symbol":"_OBJC_$_INSTANCE_VARIABLES_MHSchemaMHVoiceTriggerEventInfo"},{"value":511285791817728},{"value":0},{"value":511285791817728},{"value":2},{"value":4294967295},{"value":0},{"value":17179869184},{"value":0},{"value":2},{"value":0},{"value":0},{"value":119043},{"value":0},{"value":18446744073709551569},{"value":8522835640},{"value":0},{"value":4294967295},{"value":2},{"value":511285791817728},{"value":0},{"value":511285791817728},{"value":6431481992},{"value":8589934592,"symbolLocation":192,"symbol":"_OBJC_$_INSTANCE_VARIABLES_MHSchemaMHVoiceTriggerEventInfo"},{"value":21592279046},{"value":18446744073709550527},{"value":4412409862}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6658929568},"cpsr":{"value":4096},"fp":{"value":6431481840},"sp":{"value":6431481760},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658853940},"far":{"value":0}},"frames":[{"imageOffset":3124,"symbol":"mach_msg2_trap","symbolLocation":8,"imageIndex":82},{"imageOffset":78752,"symbol":"mach_msg2_internal","symbolLocation":76,"imageIndex":82},{"imageOffset":38756,"symbol":"mach_msg_overwrite","symbolLocation":484,"imageIndex":82},{"imageOffset":4008,"symbol":"mach_msg","symbolLocation":24,"imageIndex":82},{"imageOffset":511612,"symbol":"__CFRunLoopServiceMachPort","symbolLocation":160,"imageIndex":77},{"imageOffset":505752,"symbol":"__CFRunLoopRun","symbolLocation":1208,"imageIndex":77},{"imageOffset":502872,"symbol":"CFRunLoopRunSpecific","symbolLocation":572,"imageIndex":77},{"imageOffset":1435644,"symbol":"_NSEventThread","symbolLocation":140,"imageIndex":79},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333982,"name":"JUCE Timer","threadState":{"x":[{"value":260},{"value":0},{"value":2081660672},{"value":0},{"value":0},{"value":160},{"value":0},{"value":299998875},{"value":13688532440},{"value":0},{"value":3746560},{"value":16091352676248322},{"value":16091352676248322},{"value":3746560},{"value":0},{"value":16091352676248320},{"value":305},{"value":8522833760},{"value":0},{"value":5289674704},{"value":5289674768},{"value":13688533216},{"value":299998875},{"value":0},{"value":2081660672},{"value":2081660673},{"value":2081660928},{"value":13683085312,"symbolLocation":8,"symbol":"Steinberg::Vst::ChannelContext::IInfoListener::iid"},{"value":4294967295}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":13688532560},"sp":{"value":13688532416},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":7088160,"symbol":"juce::WaitableEvent::wait(double) const","symbolLocation":544,"imageIndex":12},{"imageOffset":7304088,"symbol":"juce::Timer::TimerThread::run()","symbolLocation":476,"imageIndex":12},{"imageOffset":7091180,"symbol":"juce::Thread::threadEntryPoint()","symbolLocation":292,"imageIndex":12},{"imageOffset":7232964,"symbol":"juce::Thread::createNativeThread(juce::Thread::Priority)::$_28::__invoke(void*)","symbolLocation":152,"imageIndex":12},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3333983,"name":"tcp_listener","threadState":{"x":[{"value":4},{"value":0},{"value":13689105956},{"value":13689106525},{"value":13681844104},{"value":12},{"value":0},{"value":0},{"value":105553191309852},{"value":8505771288,"symbolLocation":0,"symbol":"_current_pid"},{"value":340},{"value":1776},{"value":2043},{"value":2045},{"value":2326321190},{"value":2324221989},{"value":30},{"value":8522842408},{"value":0},{"value":105553185068496},{"value":105553191309824},{"value":13681844239},{"value":13643996404,"symbolLocation":0,"symbol":"tcpconnection_handleconnectionthread(_tcpconnection*)"},{"value":13681844293},{"value":13681844185},{"value":0},{"value":0},{"value":13683380224,"symbolLocation":31304,"symbol":"midiseq_clk.av"},{"value":13683077120,"symbolLocation":13552,"symbol":"g_eventcontextpool"}],"flavor":"ARM_THREAD_STATE64","lr":{"value":13646317084},"cpsr":{"value":1610616832},"fp":{"value":13689106128},"sp":{"value":13689105936},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658888812},"far":{"value":0}},"frames":[{"imageOffset":37996,"symbol":"__accept","symbolLocation":8,"imageIndex":82},{"imageOffset":7030300,"symbol":"juce::StreamingSocket::waitForNextConnection() const","symbolLocation":92,"imageIndex":12},{"imageOffset":4709412,"symbol":"tcpconnection_listenthread(_tcpconnection*)","symbolLocation":168,"imageIndex":12},{"imageOffset":3642868,"symbol":"systhread_threadproc","symbolLocation":232,"imageIndex":12},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":3336374,"name":"caulk::deferred_logger","threadState":{"x":[{"value":14},{"value":105553266193943},{"value":0},{"value":6330265703},{"value":105553266193920},{"value":22},{"value":0},{"value":0},{"value":0},{"value":4294967295},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":18446744073709551580},{"value":8522838072},{"value":0},{"value":105553270204984},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6850378952},"cpsr":{"value":2147487744},"fp":{"value":6330265472},"sp":{"value":6330265440},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658853808},"far":{"value":0}},"frames":[{"imageOffset":2992,"symbol":"semaphore_wait_trap","symbolLocation":8,"imageIndex":82},{"imageOffset":7368,"symbol":"caulk::semaphore::timed_wait(double)","symbolLocation":224,"imageIndex":85},{"imageOffset":7024,"symbol":"caulk::concurrent::details::worker_thread::run()","symbolLocation":32,"imageIndex":85},{"imageOffset":6212,"symbol":"void* caulk::thread_proxy<std::__1::tuple<caulk::thread::attributes, void (caulk::concurrent::details::worker_thread::*)(), std::__1::tuple<caulk::concurrent::details::worker_thread*>>>(void*)","symbolLocation":96,"imageIndex":85},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":10511434,"name":"AudioCalc","threadState":{"x":[{"value":14},{"value":4357607236},{"value":6129052960},{"value":68719460488},{"value":18446744072579482184},{"value":24},{"value":5077259280},{"value":0},{"value":1},{"value":3},{"value":2},{"value":274877906944},{"value":0},{"value":4},{"value":4},{"value":1},{"value":18446744073709551580},{"value":5362177552},{"value":0},{"value":105553160194464},{"value":4453173544},{"value":4453173541},{"value":4453171200},{"value":4453173540},{"value":4453171200},{"value":4451959952},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":4357607272},"cpsr":{"value":1610616832},"fp":{"value":6129053200},"sp":{"value":6129053184},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658853808},"far":{"value":0}},"frames":[{"imageOffset":2992,"symbol":"semaphore_wait_trap","symbolLocation":8,"imageIndex":82},{"imageOffset":18075496,"imageIndex":0},{"imageOffset":26792892,"imageIndex":0},{"imageOffset":18173204,"imageIndex":0},{"imageOffset":18176564,"imageIndex":0},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":10511435,"name":"AudioCalc","threadState":{"x":[{"value":14},{"value":4357607236},{"value":6131920160},{"value":68719460488},{"value":5368228992},{"value":0},{"value":5582942736},{"value":0},{"value":1},{"value":3},{"value":2},{"value":274877906944},{"value":1},{"value":2},{"value":2},{"value":3},{"value":18446744073709551580},{"value":1},{"value":0},{"value":105553160195712},{"value":4453173544},{"value":4453173541},{"value":4453171200},{"value":4453173540},{"value":4453171200},{"value":4451959952},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":4357607272},"cpsr":{"value":1610616832},"fp":{"value":6131920400},"sp":{"value":6131920384},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658853808},"far":{"value":0}},"frames":[{"imageOffset":2992,"symbol":"semaphore_wait_trap","symbolLocation":8,"imageIndex":82},{"imageOffset":18075496,"imageIndex":0},{"imageOffset":26792892,"imageIndex":0},{"imageOffset":18173204,"imageIndex":0},{"imageOffset":18176564,"imageIndex":0},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":10511436,"name":"AudioCalc","threadState":{"x":[{"value":14},{"value":4357607236},{"value":6132493600},{"value":68719460488},{"value":13958976000},{"value":58200},{"value":5366914064},{"value":0},{"value":1},{"value":2},{"value":1},{"value":274877906944},{"value":3},{"value":1},{"value":1},{"value":7},{"value":18446744073709551580},{"value":1},{"value":0},{"value":105553160203632},{"value":4453173544},{"value":4453173541},{"value":4453171200},{"value":4453173540},{"value":4453171200},{"value":4451959952},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":4357607272},"cpsr":{"value":1610616832},"fp":{"value":6132493840},"sp":{"value":6132493824},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658853808},"far":{"value":0}},"frames":[{"imageOffset":2992,"symbol":"semaphore_wait_trap","symbolLocation":8,"imageIndex":82},{"imageOffset":18075496,"imageIndex":0},{"imageOffset":26792892,"imageIndex":0},{"imageOffset":18173204,"imageIndex":0},{"imageOffset":18176564,"imageIndex":0},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":10697694,"name":"JUCE Timer","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":99999708},{"value":6135360152},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5607367096},{"value":5607367160},{"value":6135361760},{"value":99999708},{"value":0},{"value":0},{"value":45455617},{"value":45455872},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6135360272},"sp":{"value":6135360128},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":12635016,"symbol":"std::__1::cv_status std::__1::condition_variable::wait_for<long long, std::__1::ratio<1l, 1000000000l>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>> const&)","symbolLocation":388,"imageIndex":72},{"imageOffset":12634104,"symbol":"void std::__1::condition_variable::__do_timed_wait[abi:ne190102]<std::__1::chrono::steady_clock>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":72},{"imageOffset":13276428,"symbol":"std::__1::cv_status std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&)","symbolLocation":160,"imageIndex":72},{"imageOffset":13276060,"symbol":"bool std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&, juce::WaitableEvent::wait(double) const::$_1)","symbolLocation":72,"imageIndex":72},{"imageOffset":12074048,"symbol":"bool std::__1::condition_variable::wait_for[abi:ne190102]<double, std::__1::ratio<1l, 1000l>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000l>> const&, juce::WaitableEvent::wait(double) const::$_1)","symbolLocation":100,"imageIndex":72},{"imageOffset":11879848,"symbol":"juce::WaitableEvent::wait(double) const","symbolLocation":188,"imageIndex":72},{"imageOffset":11886612,"symbol":"juce::Thread::wait(double) const","symbolLocation":36,"imageIndex":72},{"imageOffset":11501556,"symbol":"juce::Timer::TimerThread::run()","symbolLocation":420,"imageIndex":72},{"imageOffset":11882736,"symbol":"juce::Thread::threadEntryPoint()","symbolLocation":244,"imageIndex":72},{"imageOffset":11883492,"symbol":"juce::juce_threadEntryPoint(void*)","symbolLocation":24,"imageIndex":72},{"imageOffset":12571352,"symbol":"juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::operator()(void*) const","symbolLocation":148,"imageIndex":72},{"imageOffset":12571192,"symbol":"juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::__invoke(void*)","symbolLocation":28,"imageIndex":72},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":10697695,"name":"SmackSampleLoader","threadState":{"x":[{"value":260},{"value":0},{"value":0},{"value":0},{"value":0},{"value":160},{"value":0},{"value":29998625},{"value":6329116824},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":5150455480},{"value":5150455544},{"value":6329118944},{"value":29998625},{"value":0},{"value":0},{"value":132624385},{"value":132624640},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6329116944},"sp":{"value":6329116800},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":12635016,"symbol":"std::__1::cv_status std::__1::condition_variable::wait_for<long long, std::__1::ratio<1l, 1000000000l>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>> const&)","symbolLocation":388,"imageIndex":72},{"imageOffset":12634104,"symbol":"void std::__1::condition_variable::__do_timed_wait[abi:ne190102]<std::__1::chrono::steady_clock>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":72},{"imageOffset":13276428,"symbol":"std::__1::cv_status std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&)","symbolLocation":160,"imageIndex":72},{"imageOffset":13276060,"symbol":"bool std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&, juce::WaitableEvent::wait(double) const::$_1)","symbolLocation":72,"imageIndex":72},{"imageOffset":12074048,"symbol":"bool std::__1::condition_variable::wait_for[abi:ne190102]<double, std::__1::ratio<1l, 1000l>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000l>> const&, juce::WaitableEvent::wait(double) const::$_1)","symbolLocation":100,"imageIndex":72},{"imageOffset":11879848,"symbol":"juce::WaitableEvent::wait(double) const","symbolLocation":188,"imageIndex":72},{"imageOffset":11886612,"symbol":"juce::Thread::wait(double) const","symbolLocation":36,"imageIndex":72},{"imageOffset":1061852,"symbol":"SmackTheDrumsAudioProcessor::SampleLoaderThread::run()","symbolLocation":244,"imageIndex":72},{"imageOffset":11882736,"symbol":"juce::Thread::threadEntryPoint()","symbolLocation":244,"imageIndex":72},{"imageOffset":11883492,"symbol":"juce::juce_threadEntryPoint(void*)","symbolLocation":24,"imageIndex":72},{"imageOffset":12571352,"symbol":"juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::operator()(void*) const","symbolLocation":148,"imageIndex":72},{"imageOffset":12571192,"symbol":"juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::__invoke(void*)","symbolLocation":28,"imageIndex":72},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":10742502,"name":"com.apple.audio.IOThread.client","threadState":{"x":[{"value":14},{"value":148879},{"value":0},{"value":0},{"value":0},{"value":0},{"value":5579235344},{"value":0},{"value":1},{"value":12882987884905038042},{"value":214404767466240},{"value":214404767466242},{"value":48},{"value":2},{"value":2},{"value":1},{"value":18446744073709551579},{"value":8522838080},{"value":0},{"value":4800740640},{"value":4800740632},{"value":5578597888},{"value":105553166302624},{"value":8505884672,"symbolLocation":0,"symbol":"AMCP::Log::Scope_Registry::get()::s_registry"},{"value":8466743296,"symbolLocation":0,"symbol":"StaticContainer<HALC_ProxyNotifications_Statics>::s_static_container_class_created"},{"value":512},{"value":8505773024,"symbolLocation":0,"symbol":"mach_task_self_"},{"value":0},{"value":6713198057}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6850495224},"cpsr":{"value":1610616832},"fp":{"value":6341324032},"sp":{"value":6341324016},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658853820},"far":{"value":0}},"frames":[{"imageOffset":3004,"symbol":"semaphore_wait_signal_trap","symbolLocation":8,"imageIndex":82},{"imageOffset":123640,"symbol":"caulk::mach::semaphore::wait_signal_or_error(caulk::mach::semaphore&)","symbolLocation":36,"imageIndex":85},{"imageOffset":2051928,"symbol":"HALC_ProxyIOContext::IOWorkLoop()","symbolLocation":5276,"imageIndex":87},{"imageOffset":2044840,"symbol":"invocation function for block in HALC_ProxyIOContext::HALC_ProxyIOContext(unsigned int, unsigned int)","symbolLocation":172,"imageIndex":87},{"imageOffset":3789692,"symbol":"HALC_IOThread::Entry(void*)","symbolLocation":88,"imageIndex":87},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":10798839,"name":"CVDisplayLink","threadState":{"x":[{"value":260},{"value":0},{"value":674279424},{"value":0},{"value":0},{"value":65704},{"value":0},{"value":15942250},{"value":753223681},{"value":0},{"value":40704},{"value":174822348857090},{"value":174822348857090},{"value":40704},{"value":0},{"value":174822348857088},{"value":305},{"value":8522833760},{"value":0},{"value":4806181432},{"value":4806181496},{"value":1},{"value":15942250},{"value":0},{"value":674279424},{"value":753223681},{"value":753223936},{"value":10044711382804},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125516},"cpsr":{"value":2684358656},"fp":{"value":6127906224},"sp":{"value":6127906080},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28940,"symbol":"_pthread_cond_wait","symbolLocation":1028,"imageIndex":83},{"imageOffset":12788,"symbol":"CVDisplayLink::waitUntil(unsigned long long)","symbolLocation":336,"imageIndex":88},{"imageOffset":8924,"symbol":"CVDisplayLink::runIOThread()","symbolLocation":500,"imageIndex":88},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]},{"id":10837208,"frames":[{"imageOffset":7020,"symbol":"start_wqthread","symbolLocation":0,"imageIndex":83}],"threadState":{"x":[{"value":6133641216},{"value":134923},{"value":6133104640},{"value":0},{"value":409604},{"value":18446744073709551615},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":0},"cpsr":{"value":4096},"fp":{"value":0},"sp":{"value":6133641216},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6659103596},"far":{"value":0}}},{"id":10837209,"frames":[{"imageOffset":7020,"symbol":"start_wqthread","symbolLocation":0,"imageIndex":83}],"threadState":{"x":[{"value":6329692160},{"value":139635},{"value":6329155584},{"value":0},{"value":409604},{"value":18446744073709551615},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":0},"cpsr":{"value":4096},"fp":{"value":0},"sp":{"value":6329692160},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6659103596},"far":{"value":0}}},{"id":10837210,"frames":[{"imageOffset":7020,"symbol":"start_wqthread","symbolLocation":0,"imageIndex":83}],"threadState":{"x":[{"value":6341898240},{"value":45779},{"value":6341361664},{"value":0},{"value":409604},{"value":18446744073709551615},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":0},"cpsr":{"value":4096},"fp":{"value":0},"sp":{"value":6341898240},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6659103596},"far":{"value":0}}},{"id":10862322,"frames":[{"imageOffset":7020,"symbol":"start_wqthread","symbolLocation":0,"imageIndex":83}],"threadState":{"x":[{"value":6130200576},{"value":146695},{"value":6129664000},{"value":0},{"value":409604},{"value":18446744073709551615},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":0},"cpsr":{"value":4096},"fp":{"value":0},"sp":{"value":6130200576},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6659103596},"far":{"value":0}}},{"id":10862669,"frames":[{"imageOffset":7020,"symbol":"start_wqthread","symbolLocation":0,"imageIndex":83}],"threadState":{"x":[{"value":6134214656},{"value":158043},{"value":6133678080},{"value":0},{"value":409604},{"value":18446744073709551615},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":0},"cpsr":{"value":4096},"fp":{"value":0},"sp":{"value":6134214656},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6659103596},"far":{"value":0}}},{"id":10862670,"frames":[{"imageOffset":7020,"symbol":"start_wqthread","symbolLocation":0,"imageIndex":83}],"threadState":{"x":[{"value":6342471680},{"value":0},{"value":6341935104},{"value":0},{"value":278532},{"value":18446744073709551615},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":0},"cpsr":{"value":4096},"fp":{"value":0},"sp":{"value":6342471680},"esr":{"value":0,"description":" Address size fault"},"pc":{"value":6659103596},"far":{"value":0}}},{"id":10865508,"name":"JUCE v8.0.12: Timer","threadState":{"x":[{"value":260},{"value":0},{"value":512},{"value":0},{"value":0},{"value":160},{"value":0},{"value":23998709},{"value":6128478936},{"value":0},{"value":0},{"value":2},{"value":2},{"value":0},{"value":0},{"value":0},{"value":305},{"value":8522833760},{"value":0},{"value":6433473512},{"value":6433473576},{"value":6128480480},{"value":23998709},{"value":0},{"value":512},{"value":513},{"value":768},{"value":0},{"value":0}],"flavor":"ARM_THREAD_STATE64","lr":{"value":6659125472},"cpsr":{"value":1610616832},"fp":{"value":6128479056},"sp":{"value":6128478912},"esr":{"value":1442840704,"description":" Address size fault"},"pc":{"value":6658868172},"far":{"value":0}},"frames":[{"imageOffset":17356,"symbol":"__psynch_cvwait","symbolLocation":8,"imageIndex":82},{"imageOffset":28896,"symbol":"_pthread_cond_wait","symbolLocation":984,"imageIndex":83},{"imageOffset":123696,"symbol":"std::__1::condition_variable::__do_timed_wait(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","symbolLocation":104,"imageIndex":86},{"imageOffset":10640172,"sourceLine":196,"sourceFile":"condition_variable.h","symbol":"std::__1::cv_status std::__1::condition_variable::wait_for<long long, std::__1::ratio<1l, 1000000000l>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>> const&)","imageIndex":75,"symbolLocation":328},{"imageOffset":10639356,"sourceLine":235,"sourceFile":"condition_variable.h","symbol":"void std::__1::condition_variable::__do_timed_wait[abi:ne190102]<std::__1::chrono::steady_clock>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<long long, std::__1::ratio<1l, 1000000000l>>>)","imageIndex":75,"symbolLocation":104},{"imageOffset":10871028,"sourceLine":161,"sourceFile":"condition_variable.h","symbol":"std::__1::cv_status std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&)","imageIndex":75,"symbolLocation":132},{"imageOffset":10870688,"sourceLine":169,"sourceFile":"condition_variable.h","symbol":"bool std::__1::condition_variable::wait_until<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000000000l>>> const&, juce::WaitableEvent::wait(double) const::$_1)","imageIndex":75,"symbolLocation":72},{"imageOffset":10410152,"sourceLine":205,"sourceFile":"condition_variable.h","symbol":"bool std::__1::condition_variable::wait_for[abi:ne190102]<double, std::__1::ratio<1l, 1000l>, juce::WaitableEvent::wait(double) const::$_1>(std::__1::unique_lock<std::__1::mutex>&, std::__1::chrono::duration<double, std::__1::ratio<1l, 1000l>> const&, juce::WaitableEvent::wait(double) const::$_1)","imageIndex":75,"symbolLocation":100},{"imageOffset":10208220,"sourceLine":55,"sourceFile":"juce_WaitableEvent.cpp","symbol":"juce::WaitableEvent::wait(double) const","imageIndex":75,"symbolLocation":188},{"imageOffset":10215972,"sourceLine":302,"sourceFile":"juce_Thread.cpp","symbol":"juce::Thread::wait(double) const","imageIndex":75,"symbolLocation":36},{"imageOffset":9780108,"sourceLine":144,"sourceFile":"juce_Timer.cpp","symbol":"juce::Timer::TimerThread::run()","imageIndex":75,"symbolLocation":420},{"imageOffset":10211772,"sourceLine":110,"sourceFile":"juce_Thread.cpp","symbol":"juce::Thread::threadEntryPoint()","imageIndex":75,"symbolLocation":340},{"imageOffset":10212736,"sourceLine":132,"sourceFile":"juce_Thread.cpp","symbol":"juce::juce_threadEntryPoint(void*)","imageIndex":75,"symbolLocation":24},{"imageOffset":10570912,"sourceLine":159,"sourceFile":"juce_Threads_mac.mm","symbol":"juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::operator()(void*) const","imageIndex":75,"symbolLocation":148},{"imageOffset":10570752,"sourceLine":143,"sourceFile":"juce_Threads_mac.mm","symbol":"juce::Thread::createNativeThread(juce::Thread::Priority)::$_0::__invoke(void*)","imageIndex":75,"symbolLocation":28},{"imageOffset":27660,"symbol":"_pthread_start","symbolLocation":136,"imageIndex":83},{"imageOffset":7040,"symbol":"thread_start","symbolLocation":8,"imageIndex":83}]}],
  "usedImages" : [
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 4339531776,
    "CFBundleShortVersionString" : "12.3.5 (2026-01-14_c4ac4719dc)",
    "CFBundleIdentifier" : "com.ableton.live",
    "size" : 102940672,
    "uuid" : "22acf5c7-4011-382b-a5d0-b782292a7879",
    "path" : "\/Applications\/Ableton Live 12 Suite.app\/Contents\/MacOS\/Live",
    "name" : "Live",
    "CFBundleVersion" : "12.3.5 (2026-01-14_c4ac4719dc)"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 4533059584,
    "size" : 12075008,
    "uuid" : "94d05e19-a04a-30b0-aa54-8db26de5faec",
    "path" : "\/Applications\/Ableton Live 12 Suite.app\/Contents\/Frameworks\/libonnxruntime_abl.dylib",
    "name" : "libonnxruntime_abl.dylib"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 4487905280,
    "size" : 98304,
    "uuid" : "f683466a-2a35-31eb-9bba-4d25d5dd42f4",
    "path" : "\/Applications\/Ableton Live 12 Suite.app\/Contents\/Frameworks\/libusb-1.0.dylib",
    "name" : "libusb-1.0.dylib"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 4490280960,
    "CFBundleIdentifier" : "se.propellerheads.rex.library",
    "size" : 294912,
    "uuid" : "ddfec8a2-ead4-3fab-9674-1be6f6e189c9",
    "path" : "\/Applications\/Ableton Live 12 Suite.app\/Contents\/Frameworks\/REX Shared Library.framework\/Versions\/A\/REX Shared Library",
    "name" : "REX Shared Library",
    "CFBundleVersion" : "1.9.1"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 4530618368,
    "size" : 49152,
    "uuid" : "d02a05cb-6440-3e7e-a02f-931734cab666",
    "path" : "\/usr\/lib\/libobjc-trampolines.dylib",
    "name" : "libobjc-trampolines.dylib"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 4782555136,
    "CFBundleShortVersionString" : "327.5",
    "CFBundleIdentifier" : "com.apple.AGXMetalG14G",
    "size" : 7372800,
    "uuid" : "ecba96b8-21ca-3241-ba32-76ebceb61cbd",
    "path" : "\/System\/Library\/Extensions\/AGXMetalG14G.bundle\/Contents\/MacOS\/AGXMetalG14G",
    "name" : "AGXMetalG14G",
    "CFBundleVersion" : "327.5"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5771362304,
    "CFBundleShortVersionString" : "0.1.0",
    "CFBundleIdentifier" : "com.Riverb.RiverbPrototype",
    "size" : 11304960,
    "uuid" : "7920faa6-b277-3397-a8d7-35d10321458e",
    "path" : "\/Users\/USER\/Library\/Audio\/Plug-Ins\/VST3\/Riverb Prototype.vst3\/Contents\/MacOS\/Riverb Prototype",
    "name" : "Riverb Prototype",
    "CFBundleVersion" : "0.1.0"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5856296960,
    "CFBundleShortVersionString" : "0.1.0",
    "CFBundleIdentifier" : "com.thievery.picosmp",
    "size" : 14237696,
    "uuid" : "6abc06a5-4c20-387b-bf08-9650ac0a02f7",
    "path" : "\/Users\/USER\/Library\/Audio\/Plug-Ins\/Components\/PicoSmp.component\/Contents\/MacOS\/PicoSmp",
    "name" : "PicoSmp",
    "CFBundleVersion" : "0.1.0"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5353062400,
    "CFBundleShortVersionString" : "1.0",
    "CFBundleIdentifier" : "com.ableton.collision",
    "size" : 262144,
    "uuid" : "7e35b71f-0b92-30ef-a767-1d9b80ed6f21",
    "path" : "\/Applications\/Ableton Live 12 Suite.app\/Contents\/App-Resources\/Devices\/Mac\/Collision.bundle\/Contents\/MacOS\/Collision",
    "name" : "Collision",
    "CFBundleVersion" : "1.0"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 5287706624,
    "CFBundleShortVersionString" : "3.0",
    "CFBundleIdentifier" : "com.apple.security.csparser",
    "size" : 131072,
    "uuid" : "c12848ee-0663-3987-842f-8832599d139f",
    "path" : "\/System\/Library\/Frameworks\/Security.framework\/Versions\/A\/PlugIns\/csparser.bundle\/Contents\/MacOS\/csparser",
    "name" : "csparser",
    "CFBundleVersion" : "61439.120.27"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5622464512,
    "CFBundleShortVersionString" : "1.0.1",
    "CFBundleIdentifier" : "com.splice.ableton-live-integration",
    "size" : 11829248,
    "uuid" : "d2d0539f-e28d-37d0-a504-cf7cd2ab8414",
    "path" : "\/Users\/USER\/Library\/Application Support\/Ableton\/*\/SpliceAbletonLive",
    "name" : "SpliceAbletonLive",
    "CFBundleVersion" : "1.0.1"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5292359680,
    "CFBundleShortVersionString" : "1.01",
    "CFBundleIdentifier" : "com.cycling74.maxchecker",
    "size" : 16384,
    "uuid" : "9b18bb58-3a99-36a2-b318-5b8f6cc06142",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/support\/maxchecker.bundle\/Contents\/MacOS\/maxchecker",
    "name" : "maxchecker",
    "CFBundleVersion" : "1.0"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 13639286784,
    "CFBundleShortVersionString" : " (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.MaxPlugLib",
    "size" : 43008000,
    "uuid" : "1ca7d171-79c9-3ddf-ad38-944ca567007a",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/support\/MaxPlugLib.bundle\/Contents\/MacOS\/MaxPlugLib",
    "name" : "MaxPlugLib",
    "CFBundleVersion" : ""
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5292441600,
    "CFBundleIdentifier" : "com.cycling74.MaxAudioAPIImpl",
    "size" : 491520,
    "uuid" : "a6fc16bd-6192-3bb6-b597-ed86c21fe6a2",
    "path" : "\/Applications\/Max.app\/Contents\/Frameworks\/MaxAudioAPIImpl.framework\/Versions\/A\/MaxAudioAPIImpl",
    "name" : "MaxAudioAPIImpl",
    "CFBundleVersion" : "1.0"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5558779904,
    "CFBundleIdentifier" : "com.cycling74.MaxLuaImpl",
    "size" : 573440,
    "uuid" : "a9399ad5-d95c-3fcc-937f-b83e8ebff0bf",
    "path" : "\/Applications\/Max.app\/Contents\/Frameworks\/MaxLuaImpl.framework\/Versions\/A\/MaxLuaImpl",
    "name" : "MaxLuaImpl",
    "CFBundleVersion" : "1.0"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5801508864,
    "CFBundleShortVersionString" : "1.7.0",
    "CFBundleIdentifier" : "com.cycling74.JitterAPIImpl",
    "size" : 1130496,
    "uuid" : "ffa7ae9f-f3d0-3a71-a250-0058dc2c6107",
    "path" : "\/Applications\/Max.app\/Contents\/Frameworks\/JitterAPIImpl.framework\/Versions\/A\/JitterAPIImpl",
    "name" : "JitterAPIImpl",
    "CFBundleVersion" : "1.7.0"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5292294144,
    "CFBundleIdentifier" : "com.cycling74.MaxLua",
    "size" : 16384,
    "uuid" : "36c7a249-8d04-3da8-8bf0-e0d50dbd63d2",
    "path" : "\/Applications\/Ableton Live 12 Suite.app\/Contents\/App-Resources\/Max\/Max.app\/Contents\/Frameworks\/MaxLua.framework\/Versions\/A\/MaxLua",
    "name" : "MaxLua",
    "CFBundleVersion" : "1.0"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5798297600,
    "size" : 1310720,
    "uuid" : "919cbda1-301b-36a8-877b-dd0e4552c71c",
    "path" : "\/Applications\/Max.app\/Contents\/Frameworks\/libmozjs185_impl.dylib",
    "name" : "libmozjs185_impl.dylib"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5353406464,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.patcher",
    "size" : 180224,
    "uuid" : "daaf82b9-3937-320a-9317-7d0bb1cac0a8",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/support\/patcher.bundle\/Contents\/MacOS\/patcher",
    "name" : "patcher",
    "CFBundleVersion" : "07b9c4cbe57"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5293162496,
    "CFBundleIdentifier" : "com.cycling74.MaxAudioAPI",
    "size" : 16384,
    "uuid" : "6f31fc51-af05-3315-adc3-d165cc3c14bb",
    "path" : "\/Applications\/Ableton Live 12 Suite.app\/Contents\/App-Resources\/Max\/Max.app\/Contents\/Frameworks\/MaxAudioAPI.framework\/Versions\/A\/MaxAudioAPI",
    "name" : "MaxAudioAPI",
    "CFBundleVersion" : "1.0"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5353930752,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.fseventwatcher",
    "size" : 16384,
    "uuid" : "72060850-0acf-3dba-9496-e39f9edce054",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/fseventwatcher.mxo\/Contents\/MacOS\/fseventwatcher",
    "name" : "fseventwatcher",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5825593344,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.cefaudio",
    "size" : 16384,
    "uuid" : "7b9d6986-da00-3351-91fe-9417c39871ce",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/msp\/cefaudio.mxo\/Contents\/MacOS\/cefaudio",
    "name" : "cefaudio",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5825675264,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.gl2",
    "size" : 1277952,
    "uuid" : "4ce3b6f8-b62a-3cd9-80d4-6edef5857e57",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/jitter\/gl2.mxo\/Contents\/MacOS\/gl2",
    "name" : "gl2",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5803802624,
    "CFBundleShortVersionString" : "1.7.0",
    "CFBundleIdentifier" : "com.cycling74.JitterAPI",
    "size" : 16384,
    "uuid" : "871ecfe7-6d82-36d4-ba8f-5fd5f42352ef",
    "path" : "\/Applications\/Ableton Live 12 Suite.app\/Contents\/App-Resources\/Max\/Max.app\/Contents\/Frameworks\/JitterAPI.framework\/Versions\/A\/JitterAPI",
    "name" : "JitterAPI",
    "CFBundleVersion" : "1.7.0"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5891391488,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.sketch",
    "size" : 671744,
    "uuid" : "08eec072-eea7-3212-bc06-f1731b9745d4",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/jitter\/sketch.mxo\/Contents\/MacOS\/sketch",
    "name" : "sketch",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5827379200,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.live.guilib",
    "size" : 229376,
    "uuid" : "5bf57c30-c8d0-31f7-b7b2-7f9984a86a8c",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/m4l\/live.guilib.mxo\/Contents\/MacOS\/live.guilib",
    "name" : "live.guilib",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5803687936,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.autohelp",
    "size" : 32768,
    "uuid" : "855f3ba2-e29d-38e4-a575-ccdc196a05b9",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/autohelp.mxo\/Contents\/MacOS\/autohelp",
    "name" : "autohelp",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5827903488,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.debugwindow",
    "size" : 16384,
    "uuid" : "e622c0b5-9c77-3944-b019-769890117926",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/debugwindow.mxo\/Contents\/MacOS\/debugwindow",
    "name" : "debugwindow",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5889671168,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.genpatcher",
    "size" : 966656,
    "uuid" : "4cec7efd-f23b-3bef-a297-21bc1121aba1",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/genpatcher.mxo\/Contents\/MacOS\/genpatcher",
    "name" : "genpatcher",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5891096576,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jsrepl",
    "size" : 16384,
    "uuid" : "118e6c16-8e04-3d63-855d-15db0cce3f68",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/jsrepl.mxo\/Contents\/MacOS\/jsrepl",
    "name" : "jsrepl",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 15533096960,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.maxclang",
    "size" : 52887552,
    "uuid" : "05cd9a5a-6136-3382-84e0-3bdfd3785203",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/maxclang.mxo\/Contents\/MacOS\/maxclang",
    "name" : "maxclang",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5893849088,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.maxurl",
    "size" : 606208,
    "uuid" : "232b3b79-a404-3e55-a418-9d902ad23a35",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/maxurl.mxo\/Contents\/MacOS\/maxurl",
    "name" : "maxurl",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 15443279872,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.maxxslt",
    "size" : 32292864,
    "uuid" : "6c320aa5-d3ce-39b6-ad64-d5c22070b2c0",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/maxxslt.mxo\/Contents\/MacOS\/maxxslt",
    "name" : "maxxslt",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5827723264,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.maxzlib",
    "size" : 98304,
    "uuid" : "ba8d17c3-f593-35ba-8395-91017cd19345",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/maxzlib.mxo\/Contents\/MacOS\/maxzlib",
    "name" : "maxzlib",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5892259840,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.objectview",
    "size" : 16384,
    "uuid" : "7ef05977-4987-3bea-9370-4fe1dc795b56",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/objectview.mxo\/Contents\/MacOS\/objectview",
    "name" : "objectview",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5892341760,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.palblocks",
    "size" : 32768,
    "uuid" : "f76d3b3d-3aa5-3042-9603-372b5eb1089e",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/palblocks.mxo\/Contents\/MacOS\/palblocks",
    "name" : "palblocks",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5892440064,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.paramview",
    "size" : 32768,
    "uuid" : "55c716da-2d69-32ff-97f0-5cf5f5596d82",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/paramview.mxo\/Contents\/MacOS\/paramview",
    "name" : "paramview",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5891178496,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.pianoroll",
    "size" : 49152,
    "uuid" : "2e319224-9b52-3bc6-9208-f74830a98676",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/pianoroll.mxo\/Contents\/MacOS\/pianoroll",
    "name" : "pianoroll",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5891293184,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.querylib",
    "size" : 32768,
    "uuid" : "9df77bbd-f9c1-3356-a049-854c4cb5ed90",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/querylib.mxo\/Contents\/MacOS\/querylib",
    "name" : "querylib",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5892751360,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.setplugpath",
    "size" : 32768,
    "uuid" : "84cba2c6-aff8-3a8a-ba12-a72756073c3b",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/setplugpath.mxo\/Contents\/MacOS\/setplugpath",
    "name" : "setplugpath",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5894619136,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.sqlite",
    "size" : 1359872,
    "uuid" : "7d18cf4b-08e6-3fb8-a5c4-0ac855f40a38",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/sqlite.mxo\/Contents\/MacOS\/sqlite",
    "name" : "sqlite",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5893455872,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.synophrys",
    "size" : 196608,
    "uuid" : "31c870d6-0e2b-31d3-bdda-a16e426e2380",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/synophrys.mxo\/Contents\/MacOS\/synophrys",
    "name" : "synophrys",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 15476129792,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.v8",
    "size" : 16285696,
    "uuid" : "07e0ae40-dc0e-3406-9a1d-16b98ff8379e",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/v8.mxo\/Contents\/MacOS\/v8",
    "name" : "v8",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5892849664,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.yaml",
    "size" : 196608,
    "uuid" : "953e2cbf-4d8d-366e-8706-9d57d6aa68d5",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/yaml.mxo\/Contents\/MacOS\/yaml",
    "name" : "yaml",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5893210112,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.zoomer",
    "size" : 16384,
    "uuid" : "0e79b788-c0a4-3d36-bfca-dd74f76f6f50",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/max\/zoomer.mxo\/Contents\/MacOS\/zoomer",
    "name" : "zoomer",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5892538368,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.max-tilde",
    "size" : 49152,
    "uuid" : "fe5d271f-f544-356a-aac4-16ed5c6d955f",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/msp\/max~.mxo\/Contents\/MacOS\/max~",
    "name" : "max~",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5893292032,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.patcherdriver",
    "size" : 32768,
    "uuid" : "1cc20ada-41de-3fe2-8558-d212efe913df",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/msp\/patcherdriver.mxo\/Contents\/MacOS\/patcherdriver",
    "name" : "patcherdriver",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5896437760,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.polybuffer",
    "size" : 49152,
    "uuid" : "f952f6ab-e0cf-39e7-bc9b-d3466502f7a6",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/msp\/polybuffer.mxo\/Contents\/MacOS\/polybuffer",
    "name" : "polybuffer",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5892669440,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.probe.history-tilde",
    "size" : 16384,
    "uuid" : "e261800b-7175-3d50-b4d6-4bbc735d25fd",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/msp\/probe.history~.mxo\/Contents\/MacOS\/probe.history~",
    "name" : "probe.history~",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5893750784,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.probe.meter-tilde",
    "size" : 16384,
    "uuid" : "6f1e5b80-6f1f-38cc-82cb-09e26c39ac8c",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/msp\/probe.meter~.mxo\/Contents\/MacOS\/probe.meter~",
    "name" : "probe.meter~",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5896552448,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.probe.scope-tilde",
    "size" : 16384,
    "uuid" : "c8820ecd-d08b-35a5-b035-3569d2efad6d",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/extensions\/msp\/probe.scope~.mxo\/Contents\/MacOS\/probe.scope~",
    "name" : "probe.scope~",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6045745152,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.gl3",
    "size" : 1720320,
    "uuid" : "75ba291c-ce3f-3723-a75f-eae1c735d4d4",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/packages\/Jitter Tools\/extensions\/gl3.mxo\/Contents\/MacOS\/gl3",
    "name" : "gl3",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5896339456,
    "CFBundleShortVersionString" : "8.2.0",
    "CFBundleIdentifier" : "com.acme.${PRODUCT_NAME:rfc1034identifier}",
    "size" : 16384,
    "uuid" : "a76b9648-661b-3884-b181-ef5485a19614",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/packages\/max-mxj\/extensions\/mxj_safe.mxo\/Contents\/MacOS\/mxj_safe",
    "name" : "mxj_safe",
    "CFBundleVersion" : "8.2.0"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6048104448,
    "CFBundleIdentifier" : "com.cycling74.rnbopatcher",
    "size" : 3850240,
    "uuid" : "b6f1e89d-b9c8-3faf-9b23-0ee1d7050007",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/packages\/RNBO\/extensions\/rnbopatcher.mxo\/Contents\/MacOS\/rnbopatcher",
    "name" : "rnbopatcher",
    "CFBundleVersion" : "1.4.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5896192000,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.comment",
    "size" : 32768,
    "uuid" : "c6c6071c-3717-3185-b207-921c7afcddfb",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/externals\/max\/comment.mxo\/Contents\/MacOS\/comment",
    "name" : "comment",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5896978432,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.sysaudio",
    "size" : 81920,
    "uuid" : "a7f77d78-9ca5-3bb6-939a-cbce2f0009da",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/packages\/VIDDLL\/extensions\/sysaudio.mxo\/Contents\/MacOS\/sysaudio",
    "name" : "sysaudio",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5896634368,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.js",
    "size" : 131072,
    "uuid" : "920d3c18-5f3f-391c-b4c0-adcbc1d75395",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/externals\/max\/js.mxo\/Contents\/MacOS\/js",
    "name" : "js",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5893390336,
    "size" : 16384,
    "uuid" : "784b3a9e-140d-3efa-9157-59705ccf1962",
    "path" : "\/Applications\/Ableton Live 12 Suite.app\/Contents\/App-Resources\/Max\/Max.app\/Contents\/Frameworks\/libmozjs185.dylib",
    "name" : "libmozjs185.dylib"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 5896863744,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jsjitter",
    "size" : 32768,
    "uuid" : "4c509340-e210-3a22-8bf9-f09aa5a6f6ce",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/jitter\/jsjitter.mxo\/Contents\/MacOS\/jsjitter",
    "name" : "jsjitter",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6045253632,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jsliveapi",
    "size" : 32768,
    "uuid" : "be0abb57-36fc-384a-9fc3-cc7de6bcf4d7",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/m4l\/jsliveapi.mxo\/Contents\/MacOS\/jsliveapi",
    "name" : "jsliveapi",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6045351936,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jsfolder",
    "size" : 16384,
    "uuid" : "9865d39a-3ad0-33a2-bdf9-2f5a6c046f72",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/max\/jsfolder.mxo\/Contents\/MacOS\/jsfolder",
    "name" : "jsfolder",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6045433856,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jsmaxobjlistener",
    "size" : 16384,
    "uuid" : "2e6d261c-2fa3-301a-897d-1a19cb069d26",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/max\/jsmaxobjlistener.mxo\/Contents\/MacOS\/jsmaxobjlistener",
    "name" : "jsmaxobjlistener",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6045515776,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jsparaminfoprovider",
    "size" : 16384,
    "uuid" : "eea3bec0-5500-3811-b12d-b66c6ef5b1b3",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/max\/jsparaminfoprovider.mxo\/Contents\/MacOS\/jsparaminfoprovider",
    "name" : "jsparaminfoprovider",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6045597696,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jsparamlistener",
    "size" : 16384,
    "uuid" : "c0658c58-a728-306d-810f-39b648c267d9",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/max\/jsparamlistener.mxo\/Contents\/MacOS\/jsparamlistener",
    "name" : "jsparamlistener",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6054936576,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jsrequire",
    "size" : 16384,
    "uuid" : "ef8c7588-6cf2-3648-bd4d-f823f6bd54af",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/max\/jsrequire.mxo\/Contents\/MacOS\/jsrequire",
    "name" : "jsrequire",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6055198720,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jsrx256",
    "size" : 16384,
    "uuid" : "9eaed4e7-d429-32d0-b776-c564e888cc6e",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/max\/jsrx256.mxo\/Contents\/MacOS\/jsrx256",
    "name" : "jsrx256",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6055280640,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jssnapshotapi",
    "size" : 16384,
    "uuid" : "dd65c8b2-4b8c-350f-b9ff-cc9c031e01a2",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/max\/jssnapshotapi.mxo\/Contents\/MacOS\/jssnapshotapi",
    "name" : "jssnapshotapi",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6055362560,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jssqlite",
    "size" : 16384,
    "uuid" : "6a41a32c-69ba-3f16-abea-959f4ae5bc00",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/max\/jssqlite.mxo\/Contents\/MacOS\/jssqlite",
    "name" : "jssqlite",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6055444480,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jssqlresult",
    "size" : 16384,
    "uuid" : "c7ec8205-8db1-3bbe-af3d-377cdb6324a6",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/max\/jssqlresult.mxo\/Contents\/MacOS\/jssqlresult",
    "name" : "jssqlresult",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6056574976,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jstrigger",
    "size" : 16384,
    "uuid" : "87b08e91-12ee-3be0-8a5c-af892e29f699",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/max\/jstrigger.mxo\/Contents\/MacOS\/jstrigger",
    "name" : "jstrigger",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6056656896,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jsxmlhttprequest",
    "size" : 16384,
    "uuid" : "84d6da4a-19b1-3fd0-adc1-80b59e4634b4",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/max\/jsxmlhttprequest.mxo\/Contents\/MacOS\/jsxmlhttprequest",
    "name" : "jsxmlhttprequest",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6056738816,
    "CFBundleShortVersionString" : "9.1.2 (07b9c4cbe57)",
    "CFBundleIdentifier" : "com.cycling74.jsbuffer",
    "size" : 16384,
    "uuid" : "5112170c-05cf-3965-a0f0-48b8ca9dfda4",
    "path" : "\/Applications\/Max.app\/Contents\/Resources\/C74\/jsextensions\/msp\/jsbuffer.mxo\/Contents\/MacOS\/jsbuffer",
    "name" : "jsbuffer",
    "CFBundleVersion" : "9.1.2"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 6369247232,
    "CFBundleShortVersionString" : "0.1.0",
    "CFBundleIdentifier" : "com.SmackTheDrums.SmackTheDrums",
    "size" : 16941056,
    "uuid" : "485bcab4-0a64-388b-b8f9-b55051da0d44",
    "path" : "\/Users\/USER\/Library\/Audio\/Plug-Ins\/VST3\/SmackTheDrums.vst3\/Contents\/MacOS\/SmackTheDrums",
    "name" : "SmackTheDrums",
    "CFBundleVersion" : "0.1.0"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 5014585344,
    "CFBundleShortVersionString" : "1.14",
    "CFBundleIdentifier" : "com.apple.audio.units.Components",
    "size" : 1294336,
    "uuid" : "674f87bc-21a4-309b-9e43-c7ba1cd95d7b",
    "path" : "\/System\/Library\/Components\/CoreAudio.component\/Contents\/MacOS\/CoreAudio",
    "name" : "CoreAudio",
    "CFBundleVersion" : "1.14"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 14092861440,
    "CFBundleShortVersionString" : "7.0",
    "CFBundleIdentifier" : "com.apple.audio.codecs.Components",
    "size" : 9322496,
    "uuid" : "42d57795-e86d-36e5-9fae-f2ba92271fba",
    "path" : "\/System\/Library\/Components\/AudioCodecs.component\/Contents\/MacOS\/AudioCodecs",
    "name" : "AudioCodecs",
    "CFBundleVersion" : "7.0"
  },
  {
    "source" : "P",
    "arch" : "arm64",
    "base" : 14143291392,
    "CFBundleShortVersionString" : "0.1.0",
    "CFBundleIdentifier" : "com.gravel.possibilitydistortion",
    "size" : 13221888,
    "uuid" : "cbce898f-88fb-3967-a815-1ede0010df08",
    "path" : "\/Library\/Audio\/Plug-Ins\/VST3\/Gravel Possibility Distortion.vst3\/Contents\/MacOS\/Gravel Possibility Distortion",
    "name" : "Gravel Possibility Distortion",
    "CFBundleVersion" : "0.1.0"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 6682578944,
    "CFBundleShortVersionString" : "6.9",
    "CFBundleIdentifier" : "com.apple.Foundation",
    "size" : 14586624,
    "uuid" : "e8f6a451-0acc-3e05-b18f-fec6618ce44a",
    "path" : "\/System\/Library\/Frameworks\/Foundation.framework\/Versions\/C\/Foundation",
    "name" : "Foundation",
    "CFBundleVersion" : "3502.1.401"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 6659575808,
    "CFBundleShortVersionString" : "6.9",
    "CFBundleIdentifier" : "com.apple.CoreFoundation",
    "size" : 5500928,
    "uuid" : "df489a59-b4f6-32b8-9bb4-9b832960aa52",
    "path" : "\/System\/Library\/Frameworks\/CoreFoundation.framework\/Versions\/A\/CoreFoundation",
    "name" : "CoreFoundation",
    "CFBundleVersion" : "3502.1.401"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 6854922240,
    "CFBundleShortVersionString" : "2.1.1",
    "CFBundleIdentifier" : "com.apple.HIToolbox",
    "size" : 3174368,
    "uuid" : "9286e29f-fcee-31d0-acea-2842ea23bedf",
    "path" : "\/System\/Library\/Frameworks\/Carbon.framework\/Versions\/A\/Frameworks\/HIToolbox.framework\/Versions\/A\/HIToolbox",
    "name" : "HIToolbox"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 6726062080,
    "CFBundleShortVersionString" : "6.9",
    "CFBundleIdentifier" : "com.apple.AppKit",
    "size" : 21568640,
    "uuid" : "5d0da1bd-412c-3ed8-84e9-40ca62fe7b42",
    "path" : "\/System\/Library\/Frameworks\/AppKit.framework\/Versions\/C\/AppKit",
    "name" : "AppKit",
    "CFBundleVersion" : "2575.60.5"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 6655295488,
    "size" : 636112,
    "uuid" : "9cf0401a-a938-389e-a77d-9e9608076ccf",
    "path" : "\/usr\/lib\/dyld",
    "name" : "dyld"
  },
  {
    "size" : 0,
    "source" : "A",
    "base" : 0,
    "uuid" : "00000000-0000-0000-0000-000000000000"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 6658850816,
    "size" : 243284,
    "uuid" : "60485b6f-67e5-38c1-aec9-efd6031ff166",
    "path" : "\/usr\/lib\/system\/libsystem_kernel.dylib",
    "name" : "libsystem_kernel.dylib"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 6659096576,
    "size" : 51784,
    "uuid" : "647b91fc-96d3-3bbb-af08-970df45257c8",
    "path" : "\/usr\/lib\/system\/libsystem_pthread.dylib",
    "name" : "libsystem_pthread.dylib"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 7135424512,
    "CFBundleShortVersionString" : "2.0",
    "CFBundleIdentifier" : "com.apple.audio.midi.CoreMIDI",
    "size" : 765216,
    "uuid" : "cf441ec3-9851-3874-8fe1-17d470031ea5",
    "path" : "\/System\/Library\/Frameworks\/CoreMIDI.framework\/Versions\/A\/CoreMIDI",
    "name" : "CoreMIDI",
    "CFBundleVersion" : "88"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 6850371584,
    "CFBundleShortVersionString" : "1.0",
    "CFBundleIdentifier" : "com.apple.audio.caulk",
    "size" : 163296,
    "uuid" : "5ce1b98f-c512-379d-9a42-5cce5923bf9d",
    "path" : "\/System\/Library\/PrivateFrameworks\/caulk.framework\/Versions\/A\/caulk",
    "name" : "caulk"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 6658150400,
    "size" : 577528,
    "uuid" : "875203a1-087b-33a6-93a5-928bb7e9114c",
    "path" : "\/usr\/lib\/libc++.1.dylib",
    "name" : "libc++.1.dylib"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 6706290688,
    "CFBundleShortVersionString" : "5.0",
    "CFBundleIdentifier" : "com.apple.audio.CoreAudio",
    "size" : 7553632,
    "uuid" : "b2b97b04-6275-3031-8a89-be6105f1e8f3",
    "path" : "\/System\/Library\/Frameworks\/CoreAudio.framework\/Versions\/A\/CoreAudio",
    "name" : "CoreAudio",
    "CFBundleVersion" : "5.0"
  },
  {
    "source" : "P",
    "arch" : "arm64e",
    "base" : 6820151296,
    "CFBundleShortVersionString" : "1.8",
    "CFBundleIdentifier" : "com.apple.CoreVideo",
    "size" : 396768,
    "uuid" : "54956113-d920-3d30-b023-f4f1419e7441",
    "path" : "\/System\/Library\/Frameworks\/CoreVideo.framework\/Versions\/A\/CoreVideo",
    "name" : "CoreVideo",
    "CFBundleVersion" : "682.6"
  }
],
  "sharedCache" : {
  "base" : 6654459904,
  "size" : 5047205888,
  "uuid" : "d7397d7f-8df9-3920-81a7-c0a144be9c51"
},
  "legacyInfo" : {
  "threadTriggered" : {
    "name" : "MainThread",
    "queue" : "com.apple.main-thread"
  }
},
  "logWritingSignature" : "287ef0160bc0a77b1f72011d3d93a65e64cf4214",
  "trialInfo" : {
  "rollouts" : [
    {
      "rolloutId" : "67181b10c68c361a728c7cfa",
      "factorPackIds" : {

      },
      "deploymentId" : 240000005
    },
    {
      "rolloutId" : "5fb4245a1bbfe8005e33a1e1",
      "factorPackIds" : {

      },
      "deploymentId" : 240000021
    }
  ],
  "experiments" : [

  ]
}
}

Model: Mac14,7, BootROM 11881.121.1, proc 8:4:4 processors, 8 GB, SMC 
Graphics: Apple M2, Apple M2, Built-In
Display: Color LCD, 2560 x 1600 Retina, Main, MirrorOff, Online
Memory Module: LPDDR5, Micron
AirPort: spairport_wireless_card_type_wifi (0x14E4, 0x4378), wl0: Dec  7 2024 04:26:10 version 18.90.5.0.7.8.177 FWID 01-52fabec4
IO80211_driverkit-1475.39 "IO80211_driverkit-1475.39" Apr 18 2025 20:10:40
AirPort: 
Bluetooth: Version (null), 0 services, 0 devices, 0 incoming serial ports
Network Service: Wi-Fi, AirPort, en0
USB Device: USB31Bus
USB Device: USB31Bus
Thunderbolt Bus: MacBook Pro, Apple Inc.
Thunderbolt Bus: MacBook Pro, Apple Inc.
