copy_component:
	mkdir -p ~/Library/Audio/Plug-Ins/Components
	cp -R "cmake-build-debug/MixBuddy_artefacts/Debug/AU/Mix Buddy.component" ~/Library/Audio/Plug-Ins/Components/

logic_pro_logs:
	log stream --predicate 'process == "Logic Pro"' --style compact