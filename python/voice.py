from gradio_client import Client, handle_file

client = Client("http://localhost:50000")

result = client.predict(
    tts_text="燕子去了，有再来的时候；杨柳枯了，有再青的时候；桃花谢了，有再开的时候。但是，聪明的，你告诉我，我们的日子为什么一去不复返呢？",
    mode_checkbox_group="3s极速复刻",
    sft_dropdown="",
    prompt_text="不过应该没问题吧，毕竟那个暗号，跟夏洛克福尔摩斯故事中出现的跳舞的小人暗号非常地相似，如果是拍下照片，目前正在观察的这位福尔摩斯迷，一定已经解开了，才对",
    prompt_wav_upload=handle_file("../sample/huiyuanai.WAV"),
    prompt_wav_record=None,
    instruct_text="",
    seed=0,
    stream=False,
    speed=1,
    api_name="/generate_audio"
)

print(result)


