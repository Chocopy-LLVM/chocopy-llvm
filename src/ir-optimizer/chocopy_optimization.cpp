//
// Created by yangy on 8/19/2021.
//

#include <chocopy_optimization.hpp>

namespace lightir {

/* The Optimization Pass Your Code Here */

void PassManager::run(bool print_ir) {
    auto i = 0;
    for (auto pass : passes_) {
        i++;
        pass->run();
        if (print_ir || pass->isPrintIR()) {
            LOG(ERROR) << ">>>>>>>>>>>> After pass " << pass->getName() << " <<<<<<<<<<<<";
            m_->print();
        }
        if (pass->getName() == "Vectorization") {
            ((Vectorization *)pass)->vectorize_num = m_->vectorize_num;
        }
        if (pass->getName() == "Multithread") {
            ((Multithread *)pass)->thread_num = m_->thread_num;
        }
    }
}
/* The Optimization Your Code Here, Uncomment the below to register your Pass */
PassManager::PassManager(Module *m) : m_(m) {
    // REGISTER_CLASS(Multithread, std::forward<Module *&>(m_));
    // REGISTER_CLASS(Vectorization, std::forward<Module *&>(m_));
    // REGISTER_CLASS(Mem2Reg, std::forward<Module *&>(m_));
    // REGISTER_CLASS(ConstPropagation, std::forward<Module *&>(m_));
    // REGISTER_CLASS(ActiveVars, std::forward<Module *&>(m_));
    // REGISTER_CLASS(LoopInvariant, std::forward<Module *&>(m_));
    // REGISTER_CLASS(LoopFind, std::forward<Module *&>(m_));
    // REGISTER_CLASS(Dominators, std::forward<Module *&>(m_));
}

void PassManager::add_pass(const string &name, bool print_ir) {
//     auto pass =reinterpret_cast<Pass *>(pass_factory.construct(name));
//     passes_.push_back(pass);
//     passes_.back()->setName(name);
//     passes_.back()->setPrintIR(print_ir);
    LOG(DEBUG) << name;
}

} // namespace lightir