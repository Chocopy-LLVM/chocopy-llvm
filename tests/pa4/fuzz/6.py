def a_():
    def b_():
        def c_():
            def d_():
                def e_():
                    def g_():
                        print ("g")
                    g_()
                    print("e")
                e_()
                print("d")
            d_()
            print("c")
        c_()
        print("b")
    b_()
    print("a")
    