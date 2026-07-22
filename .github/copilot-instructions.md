# Build and plug-in deployment

- This is a macOS JUCE audio plug-in project.
- After any source or CMake change, configure if required and build the plugin bundle targets that install to the DAW, not only the standalone target.
- For this project, rebuild the full AU/VST3 install path (for example `GravelPossibilityDistortion_All`) so the installed bundles in `/Library/Audio/Plug-Ins/Components` and `/Library/Audio/Plug-Ins/VST3` are updated.
- Always preserve AU and VST3 in the `FORMATS` list and keep `COPY_PLUGIN_AFTER_BUILD` enabled.
- The project deploys AU builds to `/Library/Audio/Plug-Ins/Components` and VST3 builds to `/Library/Audio/Plug-Ins/VST3` for immediate DAW testing. Do not redirect these outputs to build-only or user-local folders unless explicitly requested.
- If a change does not appear in the DAW, verify that the installed plugin bundles were overwritten in the system plug-in folders rather than assuming the build output alone is enough.
- If deployment fails due to macOS permissions, report the exact permission issue; do not silently disable plug-in copying.
