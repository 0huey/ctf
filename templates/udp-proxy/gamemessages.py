def Handle(data, origin):
    # do something

    print(f"{origin} sent some data: {data}")
    print("Let's reverse it")

    return data[::-1]


